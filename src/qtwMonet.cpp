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