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
    // 枚举值与DWM API对齐，使static_cast更安全
    enum MaterialType {
        Default = 0, // 对应 DWMSBT_AUTO 或 DWMSBT_NONE
        Mica = 2,    // 对应 DWMSBT_MAINWINDOW
        Acrylic = 3, // 对应 DWMSBT_TRANSIENTWINDOW
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
    // 重写事件处理器
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent* event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onThemeChanged(bool isDark);
    void onWindowTitleChanged(const QString &title);

private:
    void initialize();
    void updateCustomTheme();
    void updateFrame();

    class TitleBar;
    TitleBar *m_titleBar;

    QVBoxLayout *m_rootLayout;
    QWidget *m_contentArea;
    QWidget *m_centralWidget;

    QWPalette m_palette;
    int m_lightTone;
    int m_darkTone;
    bool m_isDarkMode;

    MaterialType m_material;
};

} // namespace QtWin

#endif // QWWINDOW_H