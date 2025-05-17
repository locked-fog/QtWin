#ifndef QTWMICA_H
#define QTWMICA_H

#include "QtWin.h"

// #include <qtwExcetion.h>
#include <dwmapi.h>
#include <windows.h>
#include <QWidget>
#include <QWindow>

namespace QtWin{
    /***
     * 启用Mica效果
     * @param widget 需要启用Mica效果的窗口
     * @return 0表示成功，其他值查看错误列表
     */
    QTWIN_API int enableMica(QWidget *widget);

    /***
     * 关闭Mica效果。注意：一般情况下当应用失去焦点时会自动失去Mica效果，无需调用此函数
     * @param widget 需要关闭Mica效果的窗口
     * @return 0表示成功，其他值查看错误列表
     */
    QTWIN_API int disableMica(QWidget *widget);

    /***
     * 启用Blur（模糊）效果
     * @param widget 需要启用模糊效果的窗口
     * @return 0表示成功
     */
    [[deprecated("Non-effect in Windows 11")]]
    QTWIN_API int enableBlur(QWidget *widget);

    /***
     * 关闭Blur效果
     * @param widget 需要关闭模糊效果的窗口
     * @return 0表示成功
     */
    [[deprecated("Non-effect in Windows 11")]]
    QTWIN_API int disableBlur(QWidget *widget);
}
#endif