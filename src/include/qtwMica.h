#ifndef QTWMICA_H
#define QTWMICA_H

#ifndef DWM_SYSTEMBACKDROP_TYPE_MICA
#define DWM_SYSTEMBACKDROP_TYPE_MICA 2
#endif

#include "include/qtwException.h"
#include <dwmapi.h>
#include <windows.h>
#include <QWidget>
#include <QWindow>


/***
 * 启用Mica效果
 * @param widget 需要启用Mica效果的窗口
 * @return 0表示成功，其他值查看错误列表
 */
int enableMica(QWidget *widget);
// int disableMica(QWidget *widget);



#endif