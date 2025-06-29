#ifndef QWWINDOW_H
#define QWWINDOW_H

#include "QtWin/QWPalette.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QShowEvent;
QT_END_NAMESPACE

namespace QtWin {

class QWWindow : public QWidget {
    Q_OBJECT

    Q_PROPERTY(QColor seedColor READ seedColor WRITE setSeedColor NOTIFY seedColorChanged)
    Q_PROPERTY(int lightTone READ lightTone WRITE setLightTone NOTIFY toneChanged)
    Q_PROPERTY(int darkTone READ darkTone WRITE setDarkTone NOTIFY toneChanged)
    Q_PROPERTY(MaterialType material READ material WRITE setMaterial NOTIFY materialChanged)

public:
    enum MaterialType {
        Default = 1,
        Mica = 2,
        Acrylic = 3,
        MicaTabbed = 4
    };
    Q_ENUM(MaterialType)

    explicit QWWindow(QWidget *parent = nullptr);
    ~QWWindow() override;

    void setCentralWidget(QWidget *widget);
    QWidget *centralWidget() const;

    QColor seedColor() const;
    int lightTone() const;
    int darkTone() const;
    MaterialType material() const;

    /**
     * @brief 获取当前窗口的调色板实例
     * @return QWPalette 引用，可用于后续控件的颜色计算
     */
    const QWPalette& colorPalette() const;

    /**
     * @brief 根据当前主题模式获取指定色彩角色和色调的颜色
     * @param colorRole 色彩角色（主色、副色等）
     * @param toneOverride 可选的色调覆盖值，如果为-1则使用当前主题的默认色调
     * @return 计算后的 QColor
     */
    QColor getThemeColor(QWPalette::QWColor colorRole, int toneOverride = -1) const;

    /**
     * @brief 获取当前主题模式下的默认色调
     * @return 当前色调值
     */
    int currentTone() const;

    /**
     * @brief 判断当前是否为深色模式
     * @return true 如果是深色模式
     */
    bool isDarkMode() const;

public slots:
    void setSeedColor(const QColor &color);
    void setMaterial(MaterialType type);
    void setLightTone(int tone);
    void setDarkTone(int tone);

signals:
    void seedColorChanged(const QColor &color);
    void materialChanged(MaterialType type);
    void toneChanged();
    void themeChanged(bool isDark);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onThemeChanged(bool isDark);

private:
    void initialize();
    void updateCustomTheme();
    void updateFrame();
    void setupPalettes();

    QVBoxLayout *m_rootLayout;
    QWidget *m_centralWidget;

    QWPalette m_palette;
    int m_lightTone;
    int m_darkTone;
    bool m_isDarkMode;
    MaterialType m_material;
    bool m_firstShow;
};

} // namespace QtWin

#endif // QWWINDOW_H