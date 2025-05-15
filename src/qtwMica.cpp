#include "include/qtwMica.h"

int enableMica(QWidget *widget){
    if (widget == nullptr){
        throw QtwException(QTWMICA_ENABLEMICA_NULLPTR);
    }
    // 获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    DWORD policy = DWM_SYSTEMBACKDROP_TYPE_MICA;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &policy, sizeof(policy));
    const MARGINS margins = {-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    return 0;
}