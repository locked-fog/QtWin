#include <qtwException.h>

QtwException::QtwException(QtwExceptionList code){
    this->exceptionCode = code;
}

const char* QtwException::what() const noexcept {
    return getErrorMessage().c_str();
}

std::string QtwException::getErrorMessage() const {
    auto it = errorMap.find(exceptionCode);
    if (it != errorMap.end()) {
        return it->second;
    } else {
        return "Undefined Exception: " + std::to_string(exceptionCode);
    }
}