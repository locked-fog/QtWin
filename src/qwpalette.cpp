#include "QtWin/QWPalette.h"

#include <cmath>

// Helper constants for D65 white point and sRGB<->XYZ matrices
static constexpr double D65_X = 95.047, D65_Y = 100.000, D65_Z = 108.883;

// sRGB to XYZ (D65), linear transform coefficients
static const double SRGB_TO_XYZ[3][3] = {
    {0.4124, 0.3576, 0.1805},
    {0.2126, 0.7152, 0.0722},
    {0.0193, 0.1192, 0.9505}
};
// XYZ to sRGB, inverse of above
static const double XYZ_TO_SRGB[3][3] = {
    { 3.2406, -1.5372, -0.4986},
    {-0.9689,  1.8758,  0.0415},
    { 0.0557, -0.2040,  1.0570}
};

/** 线性化 sRGB 分量（逆伽马）。 */
static double linearize(double c) {
    if (c <= 0.04045) return c / 12.92;
    return std::pow((c + 0.055) / 1.055, 2.4);
}
/** 反线性化 sRGB 分量（伽马校正）。 */
static double delinearize(double c) {
    if (c <= 0.0031308) return c * 12.92;
    return 1.055 * std::pow(c, 1.0/2.4) - 0.055;
}

/** XYZ -> Lab 辅助函数 f(t)。 */
static double pivotLab(double t) {
    const double delta = 6.0/29.0;
    if (t > delta*delta*delta) return std::cbrt(t);
    return t / (3*delta*delta) + 4.0/29.0;
}

/** 从 sRGB 颜色计算 HCTColor */
QtWin::HCTColor QtWin::RGB2HCT(const RGBColor& rgb) {
    // 1. 归一化并线性化 RGB
    double r = linearize(rgb.red   / 255.0);
    double g = linearize(rgb.green / 255.0);
    double b = linearize(rgb.blue  / 255.0);

    // 2. 线性 RGB -> XYZ (D65)
    //    计算 X, Y, Z，在转换矩阵作用后乘以100以匹配 Lab 公式
    double X = (SRGB_TO_XYZ[0][0]*r + SRGB_TO_XYZ[0][1]*g + SRGB_TO_XYZ[0][2]*b) * 100.0;
    double Y = (SRGB_TO_XYZ[1][0]*r + SRGB_TO_XYZ[1][1]*g + SRGB_TO_XYZ[1][2]*b) * 100.0;
    double Z = (SRGB_TO_XYZ[2][0]*r + SRGB_TO_XYZ[2][1]*g + SRGB_TO_XYZ[2][2]*b) * 100.0;

    // 3. XYZ -> Lab
    double xn = X / D65_X;
    double yn = Y / D65_Y;
    double zn = Z / D65_Z;
    double fx = pivotLab(xn);
    double fy = pivotLab(yn);
    double fz = pivotLab(zn);
    double L = 116.0 * fy - 16.0;
    double a = 500.0 * (fx - fy);
    double b_val = 200.0 * (fy - fz);
    // 将 Lab 的 L* 作为 HCT 的 Tone
    double tone = L;

    // 4. 从 Lab 的 a*, b* 计算 Hue 和 Chroma（CIELCh 角度和值）
    double hue = std::atan2(b_val, a) * 180.0 / M_PI;  // atan2 返回弧度
    if (hue < 0) hue += 360.0;
    double chroma = std::sqrt(a*a + b_val*b_val);

    return { hue, chroma, tone };
}

/** 从 HCTColor 生成 sRGB 颜色 */
QtWin::RGBColor QtWin::HCT2RGB(const HCTColor& hct) {
    double hue = hct.hue;
    double chroma = hct.chroma;
    double tone = hct.tone;

    // 限定 hue 在 [0,360)，tone 在 [0,100]
    if (hue < 0) hue = std::fmod(hue, 360.0) + 360.0;
    if (hue >= 360) hue = std::fmod(hue, 360.0);
    if (tone < 0) tone = 0;
    if (tone > 100) tone = 100;

    // 5. 构造 Lab 空间中对应的色度和色相
    //    L* = tone, a* = C * cos(hue), b* = C * sin(hue)
    double L = tone;
    double a = chroma * std::cos(hue * M_PI/180.0);
    double b_val = chroma * std::sin(hue * M_PI/180.0);

    // 6. Lab -> XYZ
    double fy = (L + 16.0) / 116.0;
    double fx = fy + (a / 500.0);
    double fz = fy - (b_val / 200.0);
    double fx3 = fx*fx*fx;
    double fz3 = fz*fz*fz;
    double fy3 = fy*fy*fy;
    const double eps = 216.0/24389.0; // = (6/29)^3
    const double kappa = 24389.0/27.0; // = (29/3)^3

    double xr = (fx3 > eps) ? fx3 : (116.0*fx - 16.0) / kappa;
    double yr = (L > (kappa * eps)) ? fy3 : L / kappa;
    double zr = (fz3 > eps) ? fz3 : (116.0*fz - 16.0) / kappa;
    double X = xr * D65_X;
    double Y = yr * D65_Y;
    double Z = zr * D65_Z;

    // 7. XYZ -> 线性 RGB
    double lr = (XYZ_TO_SRGB[0][0]*X + XYZ_TO_SRGB[0][1]*Y + XYZ_TO_SRGB[0][2]*Z) / 100.0;
    double lg = (XYZ_TO_SRGB[1][0]*X + XYZ_TO_SRGB[1][1]*Y + XYZ_TO_SRGB[1][2]*Z) / 100.0;
    double lb = (XYZ_TO_SRGB[2][0]*X + XYZ_TO_SRGB[2][1]*Y + XYZ_TO_SRGB[2][2]*Z) / 100.0;

    // 8. 反线性化并量化到 [0,255]
    int R = std::lround(std::clamp(delinearize(lr), 0.0, 1.0) * 255.0);
    int G = std::lround(std::clamp(delinearize(lg), 0.0, 1.0) * 255.0);
    int B = std::lround(std::clamp(delinearize(lb), 0.0, 1.0) * 255.0);

    return { R, G, B };
}
QtWin::RGBColor::RGBColor():RGBColor(0,0,0){}
QtWin::RGBColor::RGBColor(int red,int green,int blue):red(red),green(green),blue(blue){}
QtWin::RGBColor::RGBColor(QColor qcolor):red(qcolor.red()),green(qcolor.green()),blue(qcolor.blue()){}

QtWin::QWPalette::QWPalette() : QWPalette(HCTColor{0,0,0}){}
QtWin::QWPalette::QWPalette(RGBColor rgb) : QWPalette(RGB2HCT(rgb)){}
QtWin::QWPalette::QWPalette(HCTColor hct) : basicColor(hct){
    const HCTColor mainColor = {this->basicColor.hue,(this->basicColor.chroma / 100.0) * 10.0 + 30.0,this->basicColor.tone};
    const HCTColor subColor = {mainColor.hue , mainColor.chroma * 0.5 , mainColor.tone};
    const HCTColor neutralColor = {subColor.hue , subColor.chroma * 0.5 , subColor.tone};
    const HCTColor neutralAccent = {neutralColor.hue , neutralColor.chroma * 0.6 , neutralColor.tone};
    const HCTColor accentColor = {((mainColor.hue + 60.0)>=360.0)? (mainColor.hue - 300.0) : (mainColor.hue + 60.0), mainColor.chroma * 0.6, mainColor.tone};
    palette[0] = mainColor;
    palette[1] = subColor;
    palette[2] = neutralColor;
    palette[3] = neutralAccent;
    palette[4] = accentColor;
}

void QtWin::QWPalette::setSeedColor(RGBColor rgb){
    this->setSeedColor(RGB2HCT(rgb));
}
void QtWin::QWPalette::setSeedColor(HCTColor hct){
    this->basicColor = hct;
    const HCTColor mainColor = {this->basicColor.hue,(this->basicColor.chroma / 100.0) * 10.0 + 30.0,this->basicColor.tone};
    const HCTColor subColor = {mainColor.hue , mainColor.chroma * 0.5 , mainColor.tone};
    const HCTColor neutralColor = {subColor.hue , subColor.chroma * 0.5 , subColor.tone};
    const HCTColor neutralAccent = {neutralColor.hue , neutralColor.chroma * 0.6 , neutralColor.tone};
    const HCTColor accentColor = {((mainColor.hue + 60.0)>=360.0)? (mainColor.hue - 300.0) : (mainColor.hue + 60.0), mainColor.chroma * 0.6, mainColor.tone};
    palette[0] = mainColor;
    palette[1] = subColor;
    palette[2] = neutralColor;
    palette[3] = neutralAccent;
    palette[4] = accentColor;
}

QtWin::HCTColor QtWin::QWPalette::getHCTColor(QWColor n,int tone){
    const HCTColor hct = {this->palette[n].hue,this->palette[n].chroma,(double)tone};
    return hct;
}
QtWin::RGBColor QtWin::QWPalette::getRGBColor(QWColor n,int tone){
    const HCTColor hct = this->getHCTColor(n,tone);
    return HCT2RGB(hct);
}
QColor QtWin::QWPalette::getQColor(QWColor n,int tone){
    const RGBColor rgb = this->getRGBColor(n,tone);
    return QColor(rgb.red,rgb.green,rgb.blue);
}

//Extract color from picture
namespace QtWin{
    namespace Monet{
        //Hide details
        static void quantizeImageColors(const QImage& image,std::vector<int>& colorCount){
            //固定4096个桶
            const int kBins = 4096;

            //初始化桶
            if(colorCount.size() != kBins){
                colorCount.assign(kBins, 0);
            }

            //处理图像缩放
            constexpr int kMaxDimension = 256;
            int width = image.width();
            int height = image.height();
            int scaledWidth = width;
            int scaledHeight = height;
            if (width > kMaxDimension || height > kMaxDimension) {
                if (width > height) {
                    scaledWidth = kMaxDimension;
                    scaledHeight = (kMaxDimension * height) / width;
                } else {
                    scaledHeight = kMaxDimension;
                    scaledWidth = (kMaxDimension * width) / height;
                }
            }

            // 转换图像格式
            QImage img = (image.format() == QImage::Format_ARGB32)
                ? image
                : image.convertToFormat(QImage::Format_ARGB32);

            // 缩放处理
            if (scaledWidth != width || scaledHeight != height) {
                img = img.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }

            // 遍历
            for (int y = 0; y < img.height(); ++y) {
                const QRgb* scanLine = reinterpret_cast<const QRgb*>(img.scanLine(y));
                for (int x = 0; x < img.width(); ++x) {
                    QRgb pixel = scanLine[x];
                    // 跳过完全透明或近乎透明的像素
                    if (qAlpha(pixel) < 128) {
                        continue;
                    }
                    // 每个通道量化到4位（0-15）
                    int r = qRed(pixel) >> 4;
                    int g = qGreen(pixel) >> 4;
                    int b = qBlue(pixel) >> 4;
                    // 组合成12位键
                    int key = (r << 8) | (g << 4) | b;
                    // 对应桶加1
                    colorCount[key]++;
                }
            }
        }

        static RGBColor keyToRGB(int key) {
            int r = (key >> 8) & 0xF;
            int g = (key >> 4) & 0xF;
            int b = key & 0xF;
            // 扩展到0-255范围并加半步（+8）
            RGBColor color;
            color.red = r * 16 + 8;
            color.green = g * 16 + 8;
            color.blue = b * 16 + 8;
            return color;
        }

        static void pickTopColors(const std::vector<int>& colorCount, std::vector<HCTColor>& outColors, int topN = 5) {
            struct ScoredColor {
                double score;
                HCTColor hct;
            };

            std::vector<ScoredColor> scored;
            const int kBins = 4096;
            for (int key = 0; key < kBins; ++key) {
                int population = colorCount[key];
                if (population <= 0) continue;

                RGBColor rgb = keyToRGB(key);
                HCTColor hct = RGB2HCT(rgb);

                if (hct.chroma < 5.0 || hct.tone > 95.0 || hct.tone < 5.0) continue;

                double score = static_cast<double>(population) * hct.chroma;
                scored.push_back({score, hct});
            }

            // 处理灰阶兜底
            if (scored.empty()) {
                for (int key = 0; key < kBins; ++key) {
                    if (colorCount[key] > 0) {
                        RGBColor rgb = keyToRGB(key);
                        HCTColor hct = RGB2HCT(rgb);
                        scored.push_back((ScoredColor){colorCount[key], hct});
                    }
                }
            }

            // 选出前 topN 个颜色
            std::partial_sort(scored.begin(), scored.begin() + std::min<int>(topN, scored.size()), scored.end(),
                            [](const ScoredColor& a, const ScoredColor& b) {
                                return a.score > b.score;
                            });

            outColors.clear();
            for (int i = 0; i < std::min<int>(topN, scored.size()); ++i) {
                outColors.push_back(scored[i].hct);
            }
        }
    }

    void extractSeedColor(const QImage& image, std::vector<HCTColor>& colorSet) {
        std::vector<int> colorCount;
        Monet::quantizeImageColors(image, colorCount);
        Monet::pickTopColors(colorCount, colorSet);
    }
}