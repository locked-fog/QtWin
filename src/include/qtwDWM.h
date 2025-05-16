#ifndef QTWMICA_H
#define QTWMICA_H

#include "include/qtwException.h"
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
    int enableMica(QWidget *widget);

    /***
     * 关闭Mica效果。注意：一般情况下当应用失去焦点时会自动失去Mica效果，无需调用此函数
     * @param widget 需要关闭Mica效果的窗口
     * @return 0表示成果，其他值查看错误列表
     */
    int disableMica(QWidget *widget);
}





#endif