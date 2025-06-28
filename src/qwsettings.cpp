#include "QtWin/QWSettings.h"

#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>

namespace QtWin {

QWSettings::QWSettings(QObject* parent, const QString& configFilePath)
    : QObject(parent) {
    QString finalPath = configFilePath;

    // 如果没有提供自定义路径，则生成默认路径
    if (finalPath.isEmpty()) {
        // 使用 QStandardPaths 获取平台特定的配置目录
        // Windows: C:\Users\<User>\AppData\Local\<OrgName>\<AppName>
        const QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        
        // 确保目录存在
        QDir().mkpath(configDir);

        finalPath = configDir + "/settings.conf";
    }

    // 创建 QSettings 实例，强制使用 INI 文件格式
    m_settings.reset(new QSettings(finalPath, QSettings::IniFormat));
}

QString QWSettings::filePath() const {
    return m_settings->fileName();
}

void QWSettings::setValue(const QString& key, const QVariant& value) {
    m_settings->setValue(key, value);
}

QVariant QWSettings::value(const QString& key, const QVariant& defaultValue) const {
    return m_settings->value(key, defaultValue);
}

bool QWSettings::contains(const QString& key) const {
    return m_settings->contains(key);
}

void QWSettings::remove(const QString& key) {
    m_settings->remove(key);
}

void QWSettings::sync() {
    m_settings->sync();
}

void QWSettings::beginGroup(const QString& prefix) {
    m_settings->beginGroup(prefix);
}

void QWSettings::endGroup() {
    m_settings->endGroup();
}

void QWSettings::setFilePath(const QString& filePath) {
    QString finalPath = filePath;
    
    // If empty path provided, use default path
    if (finalPath.isEmpty()) {
        const QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir().mkpath(configDir);
        finalPath = configDir + "/settings.conf";
    }

    // Create new QSettings instance with the new path
    m_settings.reset(new QSettings(finalPath, QSettings::IniFormat));
}

} // namespace QtWin
