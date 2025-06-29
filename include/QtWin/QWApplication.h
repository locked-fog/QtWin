#ifndef QWAPPLICATION_H
#define QWAPPLICATION_H

#include "QtWin/QWLogger.h"
#include "QtWin/QWSettings.h"

#include <QApplication>
#include <QString>

QWLOGGER(qtwinDefaultLogger)

namespace QtWin {
class QWSettings;
}//前向声明

namespace QtWin {

class QWApplication : public QApplication {
    Q_OBJECT
    Q_PROPERTY(bool isDarkMode READ isDarkMode WRITE setDarkMode NOTIFY darkModeChanged)

public:
    /**
     * @brief 构造函数。
     * @param argc 命令行参数计数。
     * @param argv 命令行参数数组。
     * @param orgName 组织名称，用于 QSettings 和标准路径。
     * @param appName 应用程序名称，用于 QSettings 和标准路径。
     * @param appVersion 应用程序版本号。
     * @param isDarkMode 初始的深色模式状态。
     */
    QWApplication(int &argc, char **argv,
                const QString &orgName,
                const QString &appName,
                const QString &appVersion,
                bool isDarkMode = false);

    static QWApplication* instance();
    QWSettings* settings() const;
    bool isDarkMode() const;

public slots:
    void setDarkMode(bool dark);
    void toggleDarkMode();

signals:
    void darkModeChanged(bool isDark);

private:
    bool m_isDarkMode;
    QWSettings* m_settings;
};

} // namespace QtWin

#endif