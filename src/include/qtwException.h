#ifndef QTWEXCEPTION_H
#define QTWEXCEPTION_H

#include <bits/stdc++.h>
#include "QtWin.h"
    
/***
 * 异常信息定义方式
 * QT[来源]_[位置]_[名称]
 */
enum QtwExceptionList{
    QTWDWM_ENABLEMICA_NULLPTR = 0x00000001, // qtwDWM.cpp/enableMica(QWidget *widget)->widget == nullptr
    QTWDWM_DISABLEMICA_NULLPTR = 0x00000002, // qtwDWM.cpp/disableMica(QWidget *widget)->widget == nullptr
    QTWDWM_ENABLEBLUR_NULLPTR = 0x00000003, //qtwDWM.cpp/enableBlur(QWidget *widget)->widget == nullptr
    QTWDWM_DISABLEBLUR_NULLPTR = 0x00000004, //qtwDWM.cpp/disableBlur(QWidget *widget)->widget == nullptr
    QTWBUTTON_PAINTEVENT_NULLPTR = 0x00000005, //qtwButton.cpp/paintEvent(QPaintEvent *event)->style() == nullptr
    QTWMONET_GETWALLPAPER_NULLPTR = 0x00000006, //qtwMonet.cpp/getWallpaper()->screen == nullptr
    QTWMONET_GETWALLPAPER_NULLPIXMAP = 0x00000007, //qtwMonet.cpp/getWallpaper()->pixmap == null
    QTWMONET_GETWALLPAPER_NULLIMAGE = 0x00000008, //qtwMonet.cpp/getWallpaper()|extractDominantVibrantColor(const Qimage& image)->wallpaperImage == null
    QTWMONET_EXTRACTDOMINANTVIBRANTCOLOR_NOTFOUND = 0x00000009, //qtwMonet.cpp/extractDominantVibrantColor(const QImage& image)::Color Not Found
    QTWMONET_GENERATE_NULLIMAGE = 0x0000000a, //qtwMonet.cpp/generate()->wallpaperImage == null

};

const std::unordered_map<QtwExceptionList, std::string> errorMap = {
    {QTWDWM_ENABLEMICA_NULLPTR, "qtwDWM.cpp/enableMica(QWidget *widget)->widget == nullptr"},
    {QTWDWM_DISABLEMICA_NULLPTR, "qtwDWM.cpp/disableMica(QWidget *widget)->widget == nullptr"},
    {QTWDWM_ENABLEBLUR_NULLPTR, "qtwDWM.cpp/enableBlue(QWidget *widget)->widget == nullptr"},
    {QTWDWM_DISABLEBLUR_NULLPTR, "qtwDWM.cpp/disableBlur(QWidget *widget)->widget == nullptr"},
    {QTWBUTTON_PAINTEVENT_NULLPTR, "qtwButton.cpp/paintEvent(QPaintEvent *event)->style() == nullptr"},
    {QTWMONET_GETWALLPAPER_NULLPTR, "qtwMonet.cpp/getWallpaper()->screen == nullptr"},
    {QTWMONET_GETWALLPAPER_NULLPIXMAP, "qtwMonet.cpp/getWallpaper()->pixmap == null"},
    {QTWMONET_GETWALLPAPER_NULLIMAGE, "qtwMonet.cpp/getWallpaper()|extractDominantVibrantColor(const Qimage& image)->wallpaperImage == null"},
    {QTWMONET_EXTRACTDOMINANTVIBRANTCOLOR_NOTFOUND, "qtwMonet.cpp/extractDominantVibrantColor(const QImage& image)::Color Not Found"},
    {QTWMONET_GENERATE_NULLIMAGE, "qtwMonet.cpp/generate()->wallpaperImage == null"}

};

class QTWIN_API QtwException : public std::exception {
public:
    /***
    * 根据错误码构造异常
    * @param code 错误码
    */
    QtwException(QtwExceptionList code);

    /***
     * 获取错误信息->const char*
     * @return 错误信息
     */
    const char* what() const noexcept override;

    /***
     * 获取错误信息->std::string
     * @return 错误信息
     */
    std::string getErrorMessage() const;

private:
    QtwExceptionList exceptionCode;
};

#endif