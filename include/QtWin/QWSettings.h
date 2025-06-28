#ifndef QWSETTINGS_H
#define QWSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

namespace QtWin {

/**
 * @class QWSettings
 * @brief 一个 QSettings 的封装类，提供全局统一的配置管理服务。
 *
 * QWSettings 旨在作为应用唯一的配置访问点。它在内部管理一个 QSettings 实例，
 * 负责处理配置文件的路径解析（默认路径或自定义路径）和读写操作。
 * 通过将其作为 QWApplication 的一部分，我们确保了其生命周期与应用同步，
 * 并能通过 QWApplication::instance()->settings() 在全局安全地访问。
 */
class QWSettings : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 构造函数。
     * @param parent 父对象，通常是 QWApplication 实例。
     * @param configFilePath 可选的自定义配置文件路径。如果为空，将使用默认路径。
     */
    explicit QWSettings(QObject* parent, const QString& configFilePath = {});

    /**
     * @brief 获取当前配置文件的完整路径。
     * @return 配置文件的路径字符串。
     */
    QString filePath() const;

    // --- QSettings API 封装 ---

    /**
     * @brief 设置指定键的值。
     * @param key 设置项的键。
     * @param value 要设置的值。
     */
    void setValue(const QString& key, const QVariant& value);

    /**
     * @brief 获取指定键的值。
     * @param key 设置项的键。
     * @param defaultValue 如果键不存在，则返回此默认值。
     * @return 键对应的值，如果不存在则为 defaultValue。
     */
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;

    /**
     * @brief 检查是否存在指定的键。
     * @param key 要检查的键。
     * @return 如果键存在，返回 true；否则返回 false。
     */
    bool contains(const QString& key) const;

    /**
     * @brief 移除指定的键。
     * @param key 要移除的键。
     */
    void remove(const QString& key);

    /**
     * @brief 将任何未写入的更改写入永久存储，并重新加载所有设置。
     *
     * QSettings 通常会在事件循环中自动保存，但可以调用此函数强制立即同步。
     */
    void sync();

    /**
     * @brief 进入一个设置组。
     * @param prefix 组名。后续的键都将以此为前缀。
     */
    void beginGroup(const QString& prefix);

    /**
     * @brief 退出当前设置组。
     */
    void endGroup();

    /**
     * @brief 手动设置配置文件路径。
     * @param filePath 新的配置文件路径。如果为空，将恢复为默认路径。
     */
    void setFilePath(const QString& filePath);

private:
    // 使用智能指针管理 QSettings 对象的生命周期，确保安全。
    QScopedPointer<QSettings> m_settings;
};

} // namespace QtWin

#endif
