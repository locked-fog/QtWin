#include "include/qtwMica.h"

int QtWin::enableMica(QWidget *widget){
    if (widget == nullptr){
        throw QtwException(QTWMICA_ENABLEMICA_NULLPTR);
    }

    //启用QSS：透明背景
    widget->setStyleSheet(R"(
        QWidget {
            background-color: transparent;
        }
        )");

    // 获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    DWORD policy = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &policy, sizeof(policy));
    const MARGINS margins = {-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    return 0;
}

int QtWin::disableMica(QWidget* widget){
    if (widget == nullptr){
        throw QtwException(QTWMICA_DISABLEMICA_NULLPTR);
    }

    //关闭QSS透明背景
    widget->setStyleSheet(R"(
            QWidget {
                background-color: ;
            }
        )");

    //获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());

    //关闭Mica材质
    DWORD policy = DWMSBT_NONE;
    DwmSetWindowAttribute(hwnd,DWMWA_SYSTEMBACKDROP_TYPE,&policy,sizeof(policy));
    return 0;
}
