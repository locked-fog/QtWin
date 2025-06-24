// include/QtWin/QWButton.h

#pragma once

#include <QPushButton>
#include <QColor>
#include <QPainter>

namespace QtWin {

/**
 * @brief 一个具有 WinUI 风格外观的现代化按钮控件。
 *
 * QWButton 继承自 QPushButton，重写了绘制逻辑以实现圆角、
 * 动态颜色状态（悬停、按下等），同时保留了 QPushButton
 * 完整的功能，如信号/槽、文本和图标处理。
 */
class QWButton : public QPushButton
{
    Q_OBJECT

    // 使用 Q_PROPERTY 宏暴露自定义属性，使其可以在 Qt Designer 和 QSS 中使用。
    // 这对于创建可重用的、易于定制的组件至关重要。
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor NOTIFY hoverColorChanged)
    Q_PROPERTY(QColor pressedColor READ pressedColor WRITE setPressedColor NOTIFY pressedColorChanged)

public:
    // 继承并暴露 QPushButton 的标准构造函数
    explicit QWButton(QWidget *parent = nullptr);
    explicit QWButton(const QString &text, QWidget *parent = nullptr);
    explicit QWButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    // --- Getters for custom properties ---
    QColor backgroundColor() const;
    QColor borderColor() const;
    int cornerRadius() const;
    QColor hoverColor() const;
    QColor pressedColor() const;

public slots:
    // --- Setters for custom properties ---
    void setBackgroundColor(const QColor &color);
    void setBorderColor(const QColor &color);
    void setCornerRadius(int radius);
    void setHoverColor(const QColor &color);
    void setPressedColor(const QColor &color);

signals:
    // --- 属性变化时发出的信号 ---
    void backgroundColorChanged(const QColor &color);
    void borderColorChanged(const QColor &color);
    void cornerRadiusChanged(int radius);
    void hoverColorChanged(const QColor &color);
    void pressedColorChanged(const QColor &color);

protected:
    // 重写 paintEvent 来实现自定义绘制 [1, 2]
    void paintEvent(QPaintEvent *event) override;

    // 重写 sizeHint 来提供一个更合理的默认尺寸 [3, 4]
    QSize sizeHint() const override;

private:
    // 初始化默认样式
    void initializeDefaultStyle();

    // 存储自定义样式属性的私有成员变量
    QColor m_backgroundColor;
    QColor m_borderColor;
    QColor m_hoverColor;
    QColor m_pressedColor;
    int m_cornerRadius;
};

} // namespace QtWin