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

public slots:
    void setSeedColor(const QColor &color);
    void setMaterial(MaterialType type);
    void setLightTone(int tone);
    void setDarkTone(int tone);

signals:
    void seedColorChanged(const QColor &color);
    void materialChanged(MaterialType type);
    void toneChanged();

protected:
    // paintEvent现在只负责绘制透明背景，不再处理Default模式的绘制
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onThemeChanged(bool isDark);

private:
    /**
     * @brief 【新增】根据颜色生成QSS背景样式字符串。
     * @param color 要应用的背景颜色。
     * @return 返回一个格式为 "background-color: rgb(r, g, b);" 的QString。
     */
    QString generateColorStyleSheet(const QColor& color) const;

    void initialize();
    void updateCustomTheme();
    void updateFrame();

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