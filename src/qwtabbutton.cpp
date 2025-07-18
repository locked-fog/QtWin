#include "QtWin/QWTabButton.h"
#include "QtWin/QWPalette.h"

#include <QPainter>
#include <QPaintEvent>

namespace QtWin {

QWTabButton::QWTabButton(QWidget *parent)
    : QAbstractButton(parent),
      m_isExpanded(true)
{
    // 开启鼠标跟踪，以便在不点击的情况下也能接收 hover 事件 (mouseMoveEvent)
    setMouseTracking(true);
    // 设置为可选中状态，QButtonGroup 会利用这个属性来管理按钮的互斥点击
    setCheckable(true);
}

void QWTabButton::setIcon(const QIcon &icon) {
    m_icon = icon;
    update(); // 触发重绘以显示新图标
}

void QWTabButton::setText(const QString &text) {
    m_text = text;
    update(); // 触发重绘以显示新文本
}

QSize QWTabButton::sizeHint() const {
    // 根据展开状态提供不同的尺寸建议，确保布局正确调整
    // 固定高度为 40px，宽度根据展开状态变化
    return QSize(m_isExpanded ? 200 : 48, 40);
}

QSize QWTabButton::minimumSizeHint() const {
    // 最小尺寸与建议尺寸保持一致，防止被过度压缩
    return sizeHint();
}

void QWTabButton::setExpanded(bool expanded) {
    if (m_isExpanded != expanded) {
        m_isExpanded = expanded;
        // 状态变化后，需要通知布局系统尺寸建议可能已改变，并触发重绘
        updateGeometry();
        update();
    }
}

void QWTabButton::updateTheme(const QWPalette &palette, bool isDark) {
    // 从传入的调色板中获取所有需要的颜色
    int textTone = isDark ? 90 : 10;   // 深色模式用亮色文字，浅色模式用深色文字
    int iconTone = isDark ? 80 : 20;

    m_textColor = palette.getQColor(QWPalette::neutralColor, textTone);
    m_iconColor = palette.getQColor(QWPalette::neutralColor, iconTone);
    m_indicatorColor = palette.getQColor(QWPalette::accentColor, isDark ? 60 : 40); // 激活指示器使用强调色

    // 为悬停和选中状态使用细微变化的背景色，以提供视觉反馈
    m_hoverColor = palette.getQColor(QWPalette::neutralAccent, isDark ? 20 : 95);
    m_selectedColor = palette.getQColor(QWPalette::neutralAccent, isDark ? 30 : 90);

    update(); // 应用新的颜色
}

void QWTabButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿以获得平滑的边缘

    // --- 步骤 1: 绘制背景 ---
    // 根据按钮是否被选中 (isChecked) 或鼠标是否悬停 (underMouse) 来决定背景色
    if (isChecked()) {
        painter.fillRect(rect(), m_selectedColor);
    } else if (underMouse()) {
        painter.fillRect(rect(), m_hoverColor);
    }

    // --- 步骤 2: 绘制激活指示器 ---
    // 如果按钮被选中，则在左侧绘制一个 3px 宽的彩色竖条
    if (isChecked()) {
        painter.setPen(Qt::NoPen); // 无边框
        painter.setBrush(m_indicatorColor);
        painter.drawRect(0, height() / 4, 3, height() / 2); // 竖条高度为按钮高度的一半，居中
    }

    // --- 步骤 3: 绘制图标 ---
    const int iconSize = 20;
    const int leftPadding = 14; // 图标距离按钮左边缘的距离
    const int iconTextPadding = 12; // 图标和文字之间的距离

    if (!m_icon.isNull()) {
        QRect iconRect(leftPadding, (height() - iconSize) / 2, iconSize, iconSize);

        // 获取图标的 QPixmap，并对其重新着色
        // 这是一个简单有效的给单色图标上色的技巧
        QPixmap pixmap = m_icon.pixmap(QSize(iconSize, iconSize));
        QPainter pixmapPainter(&pixmap);
        pixmapPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        // 如果选中，图标使用指示器颜色；否则使用默认图标颜色
        pixmapPainter.fillRect(pixmap.rect(), isChecked() ? m_indicatorColor : m_iconColor);
        pixmapPainter.end();

        painter.drawPixmap(iconRect, pixmap);
    }

    // --- 步骤 4: 绘制文本 ---
    // 仅在展开状态下绘制文本
    if (m_isExpanded && !m_text.isEmpty()) {
        painter.setPen(m_textColor);
        QFont font = this->font();
        font.setPointSize(10);
        painter.setFont(font);

        // 定义文本绘制区域
        QRect textRect = rect();
        textRect.setLeft(leftPadding + iconSize + iconTextPadding);
        textRect.setRight(textRect.right() - 5); // 5px 的右边距

        // 绘制文本，垂直居中，水平居左
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }
}

} // namespace QtWin
