#include "QtWin/QWLogger.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QScopedPointer> // 用于自动、安全地管理资源生命周期
#include <iostream>      // 用于在日志系统初始化失败时回退到控制台输出

// 定义日志类别实例
Q_LOGGING_CATEGORY(logGeneral, "qtwin.general")

namespace QtWin {

namespace { // 使用匿名命名空间来隐藏内部实现细节

// 一个结构体，用于将所有日志相关的资源（文件、流、互斥锁）捆绑在一起。
struct LogResources {
    QFile file;
    QTextStream stream;
    QMutex mutex;
};

// 使用 QScopedPointer 来管理 LogResources 的实例。
// 这确保了当应用程序退出时，LogResources 的实例会被自动销毁，
// 其析构函数会被调用，从而安全地关闭文件，避免了内存泄漏。
static QScopedPointer<LogResources> logResources;

} // 匿名命名空间结束

// 自定义消息处理器函数。所有 Qt 的日志调用都会被重定向到这里。
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // 如果日志系统未初始化或初始化失败，logResources 将为空。
    // 在这种情况下，我们将消息直接输出到标准错误流（控制台），以确保日志不会丢失。
    if (!logResources) {
        std::cerr << msg.toStdString() << std::endl;
        // 如果是致命错误，在输出后立即终止程序。
        if (type == QtFatalMsg) {
            abort();
        }
        return;
    }

    // 使用 QMutexLocker 来确保对日志文件的写入是线程安全的。
    // 当多个线程同时记录日志时，这可以防止内容交错或冲突。
    const QMutexLocker locker(&logResources->mutex);

    QString levelStr;
    switch (type) {
        case QtDebugMsg:   levelStr = "DEBUG";    break;
        case QtInfoMsg:    levelStr = "INFO";     break;
        case QtWarningMsg: levelStr = "WARNING";  break;
        case QtCriticalMsg:levelStr = "ERROR"; break;
        case QtFatalMsg:   levelStr = "FATAL";    break;
    }

    // 格式化日志消息，包含时间戳、级别、类别、消息内容和源码位置。
    const QString logMessage = QString("[%1][%2][%3] %4 (%5:%6, %7)")
                            .arg(levelStr)
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                            .arg(QString(context.category).isEmpty()? "default" : context.category)
                            .arg(msg)
                            .arg(context.file)
                            .arg(context.line)
                            .arg(context.function);

    // 将格式化后的消息写入文件流。
    logResources->stream << logMessage << Qt::endl;

    // 对于致命错误，确保消息被刷新到磁盘，然后终止程序。
    if (type == QtFatalMsg) {
        logResources->stream.flush();
        abort();
    }
}

void QWLogger::init(const QString& logFilePath) {
    // 防止重复初始化
    if (logResources) {
        return;
    }

    if (!logFilePath.isEmpty()) {
        // 创建一个新的资源持有者实例
        auto resources = new LogResources;
        resources->file.setFileName(logFilePath);

        // 尝试打开日志文件
        if (resources->file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            // 文件打开成功，设置流设备
            resources->stream.setDevice(&resources->file);
            // 在 Qt 6 中，QTextStream 默认使用 UTF-8，但显式设置是个好习惯。
            resources->stream.setEncoding(QStringConverter::Utf8);
            // 将资源持有者的所有权交给 QScopedPointer
            logResources.reset(resources);
        } else {
            // 如果文件打开失败，打印警告并清理已分配的内存
            qWarning() << "Could not open log file for writing:" << logFilePath;
            delete resources;
        }
    }

    // 安装自定义消息处理器，替换 Qt 的默认处理器。
    qInstallMessageHandler(customMessageHandler);

    qwLogger(LogLevel::Info,logGeneral) << "Logger initialized. Outputting to" << (logResources? logFilePath : "Console");
}

QWLoggerHandler::QWLoggerHandler(LogLevel level, const QLoggingCategory& (*category)(),
                                const char* file, int line, const char* function)
            : m_level(level),
            m_category(category()),
            m_file(file),
            m_line(line),
            m_function(function) {}

QWLoggerHandler::~QWLoggerHandler(){
    if(!m_stream.isEmpty()){
        switch(m_level){
            case LogLevel::Debug:
                for (QLoggingCategoryMacroHolder<QtDebugMsg> qt_category(m_category); qt_category; qt_category.control = false)
                    QMessageLogger(m_file, m_line, m_function, qt_category.name()).debug() << m_stream.toStdString();
                break;
            case LogLevel::Info:
                for (QLoggingCategoryMacroHolder<QtInfoMsg> qt_category(m_category); qt_category; qt_category.control = false)
                    QMessageLogger(m_file, m_line, m_function, qt_category.name()).info() << m_stream.toStdString();
                break;
            case LogLevel::Warning:
                for (QLoggingCategoryMacroHolder<QtWarningMsg> qt_category(m_category); qt_category; qt_category.control = false)
                    QMessageLogger(m_file, m_line, m_function, qt_category.name()).warning() << m_stream.toStdString();
                break;
            case LogLevel::Error:
                for (QLoggingCategoryMacroHolder<QtCriticalMsg> qt_category(m_category); qt_category; qt_category.control = false)
                    QMessageLogger(m_file, m_line, m_function, qt_category.name()).critical() << m_stream.toStdString();
                break;
            case LogLevel::Fatal:
                for (QLoggingCategoryMacroHolder<QtFatalMsg> qt_category(m_category); qt_category; qt_category.control = false)
                    QMessageLogger(m_file, m_line, m_function, qt_category.name()).fatal() << m_stream.toStdString();
                break;
        }
    }
}

} // namespace QtWin