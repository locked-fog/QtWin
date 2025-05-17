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
};

const std::unordered_map<QtwExceptionList, std::string> errorMap = {
    {QTWDWM_ENABLEMICA_NULLPTR, "qtwDWM.cpp/enableMica(QWidget *widget)->widget == nullptr"},
    {QTWDWM_DISABLEMICA_NULLPTR, "qtwDWM.cpp/disableMica(QWidget *widget)->widget == nullptr"},
    {QTWDWM_ENABLEBLUR_NULLPTR, "qtwDWM.cpp/enableBlue(QWidget *widget)->widget == nullptr"},
    {QTWDWM_DISABLEBLUR_NULLPTR, "qtwDWM.cpp/disableBlur(QWidget *widget)->widget == nullptr"},
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