#include "QtWin/QWApplication.h"

#include <QStandardPaths>
#include <QDir>

QWLOGNAME(qtwinDefaultLogger,"qtwin.core.app")

namespace QtWin {

QWApplication::QWApplication(int &argc, char **argv,
                            const QString &orgName,
                            const QString &appName,
                            const QString &appVersion,
                            bool isDarkMode)
        : QApplication(argc, argv),
        m_isDarkMode(isDarkMode),
        m_settings(nullptr) {
    // 1. 首先设置应用信息，这对 QSettings 和 QStandardPaths 至关重要。
    QApplication::setOrganizationName(orgName);
    QApplication::setApplicationName(appName);
    QApplication::setApplicationVersion(appVersion);

    // 2. 初始化日志系统。
    // 日志文件将存储在标准的、平台特定的用户数据位置。
    const QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath); // 确保目录存在。
    const QString logFilePath = dataPath + "/app.log";
    QWLogger::init(logFilePath);
    qwLogger(LogLevel::Info,qtwinDefaultLogger)<<"App name :"<<instance()->applicationName()<<", App Version :"<<instance()->applicationVersion();

    // 3. 初始化设置系统
    // 将 this 作为父对象，确保 QSettings 的生命周期由 QApplication 管理
    m_settings = new QWSettings(this);
}

QWApplication* QWApplication::instance() {
    // QApplication::instance() 返回 QCoreApplication*，我们将其安全地转换为 QWApplication*。
    // 这是安全的，因为我们知道在 main() 中实例化的就是 QWApplication。
    return static_cast<QWApplication*>(QApplication::instance());
}

QWSettings* QWApplication::settings() const {
    return m_settings;
}

bool QWApplication::isDarkMode() const {
    return m_isDarkMode;
}

void QWApplication::setDarkMode(bool dark) {
    if (m_isDarkMode!= dark) {
        m_isDarkMode = dark;
        emit darkModeChanged(m_isDarkMode);
    }
}

void QWApplication::toggleDarkMode() {
    setDarkMode(!m_isDarkMode);
}

} // namespace QtWin