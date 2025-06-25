#ifndef QWPALETTE_H
#define QWPALETTE_H

#include <QColor>
#include <QImage>

namespace QtWin{
    /***
     * @brief HCT Color Space
     */
    struct HCTColor {
        double hue,chroma,tone;
    };

    /***
     * @brief sRGB Color Space
     */
    struct RGBColor{
        int red,green,blue;
        RGBColor();
        RGBColor(int red,int green,int blue);
        RGBColor(QColor qcolor);
    };

    /***
     * @brief convert sRGB to HCT
     * 
     * @param rgb an sRGB color struct
     * @return an HCT color struct
     */
    HCTColor RGB2HCT(const RGBColor& rgb);

    /***
     * @brief convert HCT to sRGB
     * 
     * @param hct an HCT color struct
     * @return an sRGB color struct
     */
    RGBColor HCT2RGB(const HCTColor& hct);

    /***
     * @brief QtWinPalette
     */
    class QWPalette{
        public:
            QWPalette();
            QWPalette(RGBColor rgb);
            QWPalette(HCTColor hct);

            void setSeedColor(RGBColor rgb);
            void setSeedColor(HCTColor hct);

            enum QWColor{
                mainColor     = 0,
                subColor      = 1,
                neutralColor  = 2,
                neutralAccent = 3,
                accentColor   = 4
            };

            
            HCTColor getHCTColor(QWColor n,int tone);
            QColor getQColor(QWColor n,int tone);
            RGBColor getRGBColor(QWColor n,int tone);

        private:
            HCTColor basicColor;
            HCTColor palette[5];

    };

    /***
     * @brief extract the main color from an image
     * @param image image in QImage
     * @param colorSet HCTColor vector to get returns
     */
    void extractSeedColor(const QImage& image, std::vector<HCTColor>& colorSet);
}

#endif