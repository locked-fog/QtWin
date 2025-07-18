#ifndef QWTABBUTTON_H
#define QWTABBUTTON_H

#include <QAbstractButton>
#include <QIcon>
#include <QString>

// 前向声明 (Forward declaration) Qt 内部类，减少头文件依赖
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

namespace QtWin {

// 前向声明 QWPalette，避免循环引用头文件
class QWPalette;

/**
 * @class QWTabButton
 * @brief QWNavigationView 内部使用的专用导航按钮。
 *
 * 此类提供了一个完全自定义绘制的按钮，能够显示图标和文本。
 * 它会根据导航视图展开/收起的状态、以及按钮自身是否被激活（选中）或悬停，
 * 来动态调整自身的外观，以实现现代化的导航栏按钮效果。
 */
class QWTabButton : public QAbstractButton {
    Q_OBJECT

public:
    explicit QWTabButton(QWidget *parent = nullptr);

    /**
     * @brief 设置按钮显示的图标。
     * @param icon 要设置的 QIcon 对象。
     */
    void setIcon(const QIcon &icon);

    /**
     * @brief 设置按钮显示的文本。
     * @param text 要设置的 QString 文本。
     */
    void setText(const QString &text);

    // 重写父类的虚函数，以根据按钮状态提供自定义的尺寸建议
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    /**
     * @brief 更新按钮的视觉状态为展开或收起模式。
     * @param expanded 如果导航视图处于展开状态，则为 true。
     */
    void setExpanded(bool expanded);

    /**
     * @brief 从调色板更新与主题相关的颜色。
     * @param palette 用于获取颜色的 QWPalette 调色板实例。
     * @param isDark 当前是否为深色模式。
     */
    void updateTheme(const QWPalette &palette, bool isDark);

protected:
    // 核心的绘制逻辑在此函数中实现
    void paintEvent(QPaintEvent *event) override;

private:
    QIcon m_icon;
    QString m_text;
    bool m_isExpanded;

    // 存储从调色板获取的、与主题相关的颜色
    QColor m_iconColor;         // 图标默认颜色
    QColor m_textColor;         // 文本颜色
    QColor m_hoverColor;        // 鼠标悬停时的背景色
    QColor m_selectedColor;     // 选中时的背景色
    QColor m_indicatorColor;    // 选中时左侧指示条的颜色
};

} // namespace QtWin

#endif // QWTABBUTTON_H
