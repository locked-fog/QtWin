#ifndef QWNAVIGATIONVIEW_H
#define QWNAVIGATIONVIEW_H

#include <QWidget>
#include <QList>
#include <QEvent>

// 前向声明 Qt 内部类
QT_BEGIN_NAMESPACE
class QStackedWidget;
class QVBoxLayout;
class QPropertyAnimation;
class QButtonGroup;
class QScrollArea;
class QIcon;
class QString;
QT_END_NAMESPACE

namespace QtWin {

// 前向声明 QWTabButton
class QWTabButton;

/**
 * @class QWNavigationView
 * @brief 一个灵感来源于 WinUI NavigationView 的现代化导航控件。
 *
 * 该组件在左侧提供一个可折叠的导航面板，在右侧提供一个内容区域。
 * 它被设计为 QWWindow 的中心控件，能自动适应其父窗口的主题和材质。
 * 开发者可以通过其提供的 API 轻松添加导航页面、配置页眉和页脚。
 */
class QWNavigationView : public QWidget {
    Q_OBJECT
    // 定义 QProperty，以便可以使用 Qt 属性系统（例如，用于动画）
    Q_PROPERTY(bool isExpanded READ isExpanded WRITE setExpanded)
    Q_PROPERTY(int expandedWidth READ expandedWidth WRITE setExpandedWidth)
    Q_PROPERTY(int collapsedWidth READ collapsedWidth WRITE setCollapsedWidth)

public:
    explicit QWNavigationView(QWidget *parent = nullptr);
    ~QWNavigationView() override;

    // --- 公共 API：内容管理 ---

    /**
     * @brief 添加一个主导航项及其对应的页面。
     * @param page 当此导航项被选中时要显示的内容页面控件。
     * @param icon 导航按钮的图标。
     * @param text 导航按钮的文本。
     */
    void addPage(QWidget *page, const QIcon &icon, const QString &text);

    /**
     * @brief 添加一个设置/页脚导航项及其页面。
     * 这些项会固定显示在导航面板的底部。
     * @param page 当此导航项被选中时要显示的内容页面控件。
     * @param icon 导航按钮的图标。
     * @param text 导航按钮的文本。
     */
    void addSettingsPage(QWidget *page, const QIcon &icon, const QString &text);

    /**
     * @brief 返回页眉容器的指针。
     * 开发者可以使用此指针向页眉区域添加自定义控件（如标题、折叠按钮等）。
     * @return 指向页眉容器的 QWidget 指针。
     */
    QWidget* headerWidget() const;

    /**
     * @brief 返回页脚容器的指针。
     * 开发者可以使用此指针在设置项下方添加自定义控件（如版权信息）。
     * @return 指向页脚容器的 QWidget 指针。
     */
    QWidget* footerWidget() const;

    // --- 公共 API：状态与外观 ---

    bool isExpanded() const;
    int expandedWidth() const;
    int collapsedWidth() const;

public slots:
    /**
     * @brief 以编程方式设置导航面板的展开/收起状态（带动画）。
     * @param expanded 为 true 则展开，为 false 则收起。
     */
    void setExpanded(bool expanded);

    /**
     * @brief 切换导航面板的展开/收起状态。
     */
    void toggleExpansion();

    /**
     * @brief 设置导航面板展开时的宽度。
     * @param width 宽度值（像素）。
     */
    void setExpandedWidth(int width);

    /**
     * @brief 设置导航面板收起时的宽度。
     * @param width 宽度值（像素）。
     */
    void setCollapsedWidth(int width);

signals:
    /**
     * @brief 当当前显示的页面发生变化时发射此信号。
     * @param index 新的当前页面的索引。
     */
    void currentPageChanged(int index);

protected:
    // 重写事件处理器以实现自定义行为
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    // 私有槽函数，用于内部逻辑处理
    void _onTabClicked(int id);
    void _onThemeChanged(bool isDark);

private:
    // 私有辅助函数
    void _initializeUI();
    void _updateTheme();
    void _addTabButton(QWTabButton *button, QWidget *page, bool isSettings);

    // 主要 UI 组件
    QWidget *m_navPane;             // 左侧导航面板
    QStackedWidget *m_contentFrame; // 右侧内容区域

    // 导航面板的内部分区
    QWidget *m_headerContainer;         // 页眉容器
    QScrollArea *m_bodyScrollArea;      // 主导航项的滚动区域
    QWidget *m_bodyContainer;           // 容纳主导航按钮的控件
    QWidget *m_footerContainer;         // 容纳设置/页脚按钮的控件
    QWidget *m_customFooterContainer;   // 容纳用户自定义页脚内容的控件

    // 状态与动画
    bool m_isExpanded;
    int m_expandedWidth;
    int m_collapsedWidth;
    QPropertyAnimation *m_animation;

    // 按钮和页面管理
    QButtonGroup *m_buttonGroup;        // 用于管理按钮的互斥选择
    QList<QWTabButton*> m_allButtons;   // 用于统一管理所有按钮的引用
};

} // namespace QtWin

#endif // QWNAVIGATIONVIEW_H
