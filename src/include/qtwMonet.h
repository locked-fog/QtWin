#ifndef QTWMONET_H
#define QTWMONET_H

#include <qtwException.h>
#include <QObject>
#include <QColor>
#include <QImage>
#include <vector>
#include <utility>

#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <algorithm>
#include <map>

class QScreen;

class QTWIN_API QtwMonet : public QObject{
    Q_OBJECT

public:
    explicit QtwMonet(QObject *parent = nullptr);
    ~QtwMonet();

    void setWallpaper(const QImage& image);
    void getWallpaper();
    void generate();
    std::vector<std::pair<QColor,QColor>> getColor() const;

private:
    QImage wallpaperImage;
    QColor seedColor;
    std::vector<std::pair<QColor,QColor>> colorPalette;

    QColor extractDominantVibrantColor(const QImage& image);
    std::vector<std::pair<QColor,QColor>> generatePaletteFromSeed(const QColor& seed);
};
#endif