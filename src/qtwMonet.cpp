#include <qtwMonet.h>

const qreal SATURATION_THRESHOLD_VIBRANT = 0.35f;
const qreal VALUE_THRESHOLD_VIBRANT = 0.30f;
const qreal MAX_VALUE_THRESHOLD_VIBRANT = 0.96f;

QtwMonet::QtwMonet(QObject *parent) : QObject(parent){

}

QtwMonet::~QtwMonet(){

}

void QtwMonet::setWallpaper(const QImage& image){
    wallpaperImage = image;
    if(wallpaperImage.format()!= QImage::Format_ARGB32_Premultiplied &&
       wallpaperImage.format()!= QImage::Format_ARGB32){
        wallpaperImage = wallpaperImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
}

void QtwMonet::getWallpaper(){
    QScreen *screen = QGuiApplication::primaryScreen();
    if(!screen){
        throw(new QtwException(QTWMONET_GETWALLPAPER_NULLPTR));
    }

    QPixmap pixmap = screen->grabWindow(0);
    if(pixmap.isNull()){
        throw(new QtwException(QTWMONET_GETWALLPAPER_NULLPIXMAP));
    }

    wallpaperImage = pixmap.toImage();
    if(wallpaperImage.isNull()){
        throw(new QtwException(QTWMONET_GETWALLPAPER_NULLIMAGE));
    }

    if(wallpaperImage.format()!= QImage::Format_ARGB32_Premultiplied &&
       wallpaperImage.format()!= QImage::Format_ARGB32){
        wallpaperImage = wallpaperImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
}

QColor QtwMonet::extractDominantVibrantColor(const QImage& image){
    if (image.isNull()){
        throw(QtwException(QTWMONET_GETWALLPAPER_NULLIMAGE));
    }

    qreal maxScore = -1.0;
    QColor dominantColor;
    bool foundVibrantPixel =false;

    const int imageWidth = image.width();
    const int imageHeight = image.height();

    for(int y = 0;y<imageHeight;y++){
        const QRgb *scanLine = reinterpret_cast<const QRgb *>(image.scanLine(y));
        for(int x = 0;x < imageWidth;x++){
            QColor pixelColor(scanLine[x]);

            if(pixelColor.alpha() < 200){
                continue;
            }

            float hue, saturation, value;
            pixelColor.getHsvF(&hue,&saturation,&value);
            
            if( saturation >= SATURATION_THRESHOLD_VIBRANT &&
                value >= VALUE_THRESHOLD_VIBRANT &&
                value <= MAX_VALUE_THRESHOLD_VIBRANT){
                foundVibrantPixel = true;
                qreal currentScore = saturation * 0.7 + value * 0.3;

                if(currentScore > maxScore){
                    maxScore = currentScore;
                    dominantColor = pixelColor;
                }
            }
        }
    }

    if(foundVibrantPixel){
        return dominantColor;
    }else{
        throw(QtwException(QTWMONET_EXTRACTDOMINANTVIBRANTCOLOR_NOTFOUND));
    }
}

std::vector<std::pair<QColor, QColor>> QtwMonet::generatePaletteFromSeed(const QColor& seed){
    std::vector<std::pair<QColor,QColor>> palette(5);
    if(!seed.isValid()){
        throw(QtwException(QTWMONET_GENERATEPALETTEFROMSEED_INVALIDCOLOR));
    }
    float baseHue, baseSaturation, baseLightness;
    seed.getHslF(&baseHue,&baseSaturation,&baseLightness);

    if(baseHue < 0.0) baseHue = 0.0;
    baseSaturation = std::max(baseSaturation , 0.5f);
    baseLightness = std::max(baseLightness,0.4f);
    baseLightness = std::min(baseLightness, 0.75f);

    //Pair 0: Primary
    palette[0].first.setHslF(baseHue,std::clamp(baseSaturation, 0.5f,0.9f),std::clamp(baseLightness,0.55f,0.75f));
    palette[0].second.setHslF(baseHue, std::clamp(baseSaturation * 0.7f,0.3f,0.7f), std::clamp(baseLightness * 0.3f,0.1f,0.25f));

    //Pair 1
    float hue1 = std::fmod(baseHue + 30.0f/360.0f+1.0f,1.0f);
    palette[1].first.setHslF(hue1,std::clamp(baseSaturation * 0.9f,0.55f,0.9f),std::clamp(baseLightness*1.05f,0.6f,0.8f));
    palette[1].second.setHslF(hue1, std::clamp(baseSaturation * 0.6f, 0.3f, 0.65f), std::clamp(baseLightness * 0.35f, 0.12f, 0.28f));

    //Pair 2
    float hue2 = std::fmod(baseHue + 60.0f/360.0f + 1.0f, 1.0f);
    palette[2].first.setHslF(hue2, std::clamp(baseSaturation * 0.95f, 0.6f, 0.95f), std::clamp(baseLightness * 1.1f, 0.65f, 0.85f));
    palette[2].second.setHslF(hue2, std::clamp(baseSaturation * 0.5f, 0.25f, 0.6f), std::clamp(baseLightness * 0.4f, 0.15f, 0.3f));

    //Pair 3
    float hue3 = std::fmod(baseHue + 180.0/360.0 + 1.0, 1.0);
    palette[3].first.setHslF(hue3, std::clamp(baseSaturation * 0.8, 0.45, 0.85), std::clamp(baseLightness * 0.9, 0.5, 0.7));
    palette[3].second.setHslF(hue3, std::clamp(baseSaturation * 0.65, 0.25, 0.6), std::clamp(baseLightness * 0.25, 0.08, 0.22));

    //Pair 4
    palette[4].first.setHslF(baseHue, std::clamp(baseSaturation * 0.2, 0.05, 0.3), std::clamp(baseLightness * 1.2, 0.75, 0.9));
    palette[4].second.setHslF(baseHue, std::clamp(baseSaturation * 0.15, 0.02, 0.25), std::clamp(baseLightness * 0.15, 0.05, 0.15));
    
    return palette;
}

void QtwMonet::generate(){
    if(wallpaperImage.isNull()){
        throw(QtwException(QTWMONET_GENERATE_NULLIMAGE));
    }
    seedColor = extractDominantVibrantColor(wallpaperImage);
    colorPalette = generatePaletteFromSeed(seedColor);
}

std::vector<std::pair<QColor,QColor>> QtwMonet::getColor() const{
    return colorPalette;
}