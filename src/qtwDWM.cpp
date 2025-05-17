#include <qtwDWM.h>
#include <qtwException.h>

int QtWin::enableMica(QWidget *widget){
    if (widget == nullptr){
        throw QtwException(QTWDWM_ENABLEMICA_NULLPTR);
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
        throw QtwException(QTWDWM_DISABLEMICA_NULLPTR);
    }

    //关闭QSS透明背景
    widget->setStyleSheet(R"(
            
        )");

    //获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());

    //关闭Mica材质
    DWORD policy = DWMSBT_NONE;
    DwmSetWindowAttribute(hwnd,DWMWA_SYSTEMBACKDROP_TYPE,&policy,sizeof(policy));
    return 0;
}

int QtWin::enableBlur(QWidget *widget){
    if (widget == nullptr){
        throw QtwException(QTWDWM_ENABLEBLUR_NULLPTR);
    }
    //启用QSS：透明背景
    widget->setStyleSheet(R"(
        QWidget {
            background-color: transparent;
        }
        )");

    //获取句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    const DWM_BLURBEHIND dwm_blurbehind = {
        DWM_BB_ENABLE + DWM_BB_BLURREGION + DWM_BB_TRANSITIONONMAXIMIZED,   //启用所有控制语句
        true,   //启用模糊效果
        NULL,   //应用于整个窗口
        true    //适配最大化（？）
    };
    DwmEnableBlurBehindWindow(hwnd,&dwm_blurbehind);
    return 0;
}

int QtWin::disableBlur(QWidget *widget){
    if (widget == nullptr){
        throw QtwException(QTWDWM_DISABLEBLUR_NULLPTR);
    }
    //关闭QSS透明背景
    widget->setStyleSheet(R"(
            
        )");

    //获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    const DWM_BLURBEHIND dwm_blurbehind = {
        DWM_BB_ENABLE + DWM_BB_BLURREGION + DWM_BB_TRANSITIONONMAXIMIZED,
        false,
        NULL,
        true
    };
    DwmEnableBlurBehindWindow(hwnd,&dwm_blurbehind);
    return 0;
}