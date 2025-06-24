// src/qwbutton.cpp

#include "QtWin/QWButton.h"
#include <QPainterPath>

namespace QtWin {

// --- 构造函数 ---

QWButton::QWButton(QWidget *parent)
    : QPushButton(parent)
{
    initializeDefaultStyle();
}

QWButton::QWButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    initializeDefaultStyle();
}

QWButton::QWButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    initializeDefaultStyle();
}

// --- 私有辅助函数 ---

void QWButton::initializeDefaultStyle()
{
    // 设置一个类似 WinUI 的默认样式
    m_backgroundColor = QColor(240, 240, 240);
    m_borderColor = Qt::transparent;
    m_cornerRadius = 8;

    // 自动生成悬停和按下状态的颜色，使其与背景色关联
    // 这样用户只需设置一个基础色，就能获得一致的状态变化效果
    m_hoverColor = m_backgroundColor.lighter(110);
    m_pressedColor = m_backgroundColor.darker(115);

    // 为了让自定义绘制生效，必须禁用默认的样式表绘制
    // 并确保背景是可绘制的
    setAttribute(Qt::WA_StyledBackground, false);
}

// --- Getters ---

QColor QWButton::backgroundColor() const { return m_backgroundColor; }
QColor QWButton::borderColor() const { return m_borderColor; }
int QWButton::cornerRadius() const { return m_cornerRadius; }
QColor QWButton::hoverColor() const { return m_hoverColor; }
QColor QWButton::pressedColor() const { return m_pressedColor; }

// --- Setters ---

void QWButton::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor!= color) {
        m_backgroundColor = color;
        update(); // 请求重绘以应用新颜色 [5]
        emit backgroundColorChanged(color);
    }
}

void QWButton::setBorderColor(const QColor &color)
{
    if (m_borderColor!= color) {
        m_borderColor = color;
        update();
        emit borderColorChanged(color);
    }
}

void QWButton::setCornerRadius(int radius)
{
    if (m_cornerRadius!= radius) {
        m_cornerRadius = radius;
        update();
        emit cornerRadiusChanged(radius);
    }
}

void QWButton::setHoverColor(const QColor &color)
{
    if (m_hoverColor!= color) {
        m_hoverColor = color;
        update();
        emit hoverColorChanged(color);
    }
}

void QWButton::setPressedColor(const QColor &color)
{
    if (m_pressedColor!= color) {
        m_pressedColor = color;
        update();
        emit pressedColorChanged(color);
    }
}

// --- 重写的保护方法 ---

QSize QWButton::sizeHint() const
{
    // 在基类提供的尺寸基础上增加一些垂直和水平填充，使按钮看起来更舒适
    QSize hint = QPushButton::sizeHint();
    hint.setHeight(hint.height() + 12);
    hint.setWidth(qMax(hint.width() + 20, 100)); // 确保最小宽度
    return hint;
}

void QWButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 我们将重绘整个控件，所以不需要使用 event 中的特定区域

    QPainter painter(this);
    // 启用抗锯齿以获得平滑的圆角边缘 [2]
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 1. 根据按钮状态确定当前要使用的颜色
    QColor currentBgColor = m_backgroundColor;
    if (!isEnabled()) {
        // 禁用状态下使用一个更灰暗的背景色
        currentBgColor = QColor(220, 220, 220, 150);
    } else if (isDown()) {
        // 按下状态
        currentBgColor = m_pressedColor;
    } else if (underMouse()) {
        // 悬停状态
        currentBgColor = m_hoverColor;
    }

    // 2. 绘制背景和边框
    // 使用 QPainterPath 来创建圆角矩形，这是最灵活和高质量的方法 [2]
    QPainterPath path;
    // 使用 rect() 而不是 geometry()，因为 QPainter 在控件的局部坐标系中工作 [1]
    path.addRoundedRect(rect(), m_cornerRadius, m_cornerRadius);

    painter.save(); // 保存当前 painter 状态

    // 填充背景
    painter.setBrush(currentBgColor);
    // 设置边框画笔
    painter.setPen(QPen(m_borderColor, 1));
    painter.drawPath(path);

    painter.restore(); // 恢复 painter 状态

    // 3. 绘制文本和图标
    // 这是最巧妙的部分：我们不重新发明轮子。
    // 在绘制完我们的自定义背景后，直接调用基类 QPushButton 的 paintEvent。
    // 这样，Qt 就会为我们处理所有复杂的文本对齐、图标放置、
    // 焦点框绘制等逻辑，确保 QWButton 的行为与标准按钮完全一致。
    QPushButton::paintEvent(event);
}

} // namespace QtWin