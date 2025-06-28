#ifndef QWLOGGER_H
#define QWLOGGER_H

#include <QObject>
#include <QString>
#include <QLoggingCategory>

// 为整个日志系统定义一个总的日志类别
Q_DECLARE_LOGGING_CATEGORY(logGeneral)

namespace QtWin {

// 日志级别枚举，直接映射到 Qt 的内部类型
enum class LogLevel {
    Debug = QtDebugMsg,
    Info = QtInfoMsg,
    Warning = QtWarningMsg,
    Error = QtCriticalMsg,
    Fatal = QtFatalMsg
};

/**
 * @class QWLogger
 * @brief 一个静态工具类，用于初始化和配置应用的全局日志系统。
 *
 * 通过调用静态的 init() 方法，可以安装一个自定义的消息处理器，
 * 将所有 Qt 日志（qDebug, qInfo 等）重定向到控制台或指定的文件。
 */
class QWLogger {
public:
    // 禁止实例化，这是一个纯静态类
    QWLogger() = delete;

    /**
     * @brief 初始化日志系统。
     * @param logFilePath 如果提供，日志将写入此文件。如果为空，则写入控制台。
     *
     * 此方法应在 main() 函数中，创建 QWApplication 实例后立即调用。
     * 它会安装一个全局的消息处理器。
     */
    static void init(const QString& logFilePath = {});
};

} // namespace QtWin


#endif