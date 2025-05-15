#ifndef qtwEXCEPTION_H
#define qtwEXCEPTION_H

#include <bits/stdc++.h>


    
/***
 * 异常信息定义方式
 * QT[来源]_[位置]_[名称]
 */
enum QtwExceptionList{
    QTWMICA_ENABLEMICA_NULLPTR = 0x00000001 // qtwMica.cpp/enableMica(QWidget *widget)->widget == nullptr
};

const std::unordered_map<QtwExceptionList, std::string> errorMap = {
    {QTWMICA_ENABLEMICA_NULLPTR, "qtwMica.cpp/enableMica(QWidget *widget)->widget == nullptr"}
};

class QtwException : public std::exception {
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