#include "QtWin/QWNavigationView.h"
#include "QtWin/QWTabButton.h"
#include "QtWin/QWWindow.h"        // 需要包含以获取 QWPalette
#include "QtWin/QWApplication.h"   // 需要包含以连接主题变化信号

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QButtonGroup>
#include <QScrollArea>
#include <QPaintEvent>
#include <QPainter>
#include <QShowEvent>
#include <QFrame>

namespace QtWin {

QWNavigationView::QWNavigationView(QWidget *parent)
    : QWidget(parent),
      m_isExpanded(true),
      m_expandedWidth(280),
      m_collapsedWidth(48),
      m_animation(nullptr),
      m_buttonGroup(nullptr)
{
    _initializeUI();

    // 连接到全局的应用主题变化信号，以便自动更新外观
    if (QWApplication::instance()) {
        connect(QWApplication::instance(), &QWApplication::darkModeChanged,
                this, &QWNavigationView::_onThemeChanged);
    }
}

QWNavigationView::~QWNavigationView() = default;

void QWNavigationView::_initializeUI() {
    // 主布局：水平布局，无边距，无间距
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- 1. 左侧导航面板 (m_navPane) ---
    m_navPane = new QWidget(this);
    m_navPane->setObjectName("navPane");
    m_navPane->setFixedWidth(m_expandedWidth); // 初始为展开宽度

    auto *navPaneLayout = new QVBoxLayout(m_navPane);
    navPaneLayout->setContentsMargins(0, 0, 0, 0);
    navPaneLayout->setSpacing(0);

    // 页眉容器，用于放置标题、logo、折叠按钮等
    m_headerContainer = new QWidget(m_navPane);
    m_headerContainer->setMinimumHeight(48);
    m_headerContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 主体滚动区域，当导航项过多时可以滚动
    m_bodyScrollArea = new QScrollArea(m_navPane);
    m_bodyScrollArea->setWidgetResizable(true);    // 允许内部控件自动填充区域
    m_bodyScrollArea->setFrameShape(QFrame::NoFrame); // 无边框
    m_bodyScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 禁用水平滚动条
    m_bodyScrollArea->setObjectName("bodyScrollArea");

    // 容纳主导航按钮的容器
    m_bodyContainer = new QWidget(m_bodyScrollArea);
    m_bodyContainer->setObjectName("bodyContainer");
    auto *bodyLayout = new QVBoxLayout(m_bodyContainer);
    bodyLayout->setContentsMargins(0, 8, 0, 8); // 上下留出 8px 边距
    bodyLayout->setSpacing(4);                  // 按钮间距 4px
    bodyLayout->addStretch();                   // 添加一个弹簧，使按钮始终靠上对齐
    m_bodyScrollArea->setWidget(m_bodyContainer);

    // 容纳设置/页脚按钮的容器
    m_footerContainer = new QWidget(m_navPane);
    auto *footerLayout = new QVBoxLayout(m_footerContainer);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setSpacing(4);

    // 容纳用户自定义页脚内容的容器
    m_customFooterContainer = new QWidget(m_navPane);

    // 将各部分添加到导航面板的垂直布局中
    navPaneLayout->addWidget(m_headerContainer);
    navPaneLayout->addWidget(m_bodyScrollArea, 1); // 滚动区域占据所有剩余空间
    navPaneLayout->addWidget(m_footerContainer);
    navPaneLayout->addWidget(m_customFooterContainer);

    // --- 2. 右侧内容框架 (m_contentFrame) ---
    m_contentFrame = new QStackedWidget(this);

    // --- 3. 将导航面板和内容框架添加到主布局 ---
    mainLayout->addWidget(m_navPane);
    mainLayout->addWidget(m_contentFrame, 1); // 内容框架占据所有剩余空间

    // --- 4. 按钮组，用于管理 Tab 按钮的互斥选择 ---
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true); // 开启互斥，确保一次只有一个按钮被选中
    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &QWNavigationView::_onTabClicked);

    // --- 5. 用于实现展开/收起动画的属性动画 ---
    // 以导航面板的 minimumWidth 属性为目标进行动画
    m_animation = new QPropertyAnimation(m_navPane, "minimumWidth", this);
    m_animation->setDuration(250); // 动画时长 250ms
    m_animation->setEasingCurve(QEasingCurve::InOutCubic); // 使用平滑的缓动曲线
}

void QWNavigationView::addPage(QWidget *page, const QIcon &icon, const QString &text) {
    auto *button = new QWTabButton(this);
    button->setIcon(icon);
    button->setText(text);
    _addTabButton(button, page, false);
}

void QWNavigationView::addSettingsPage(QWidget *page, const QIcon &icon, const QString &text) {
    auto *button = new QWTabButton(this);
    button->setIcon(icon);
    button->setText(text);
    _addTabButton(button, page, true);
}

void QWNavigationView::_addTabButton(QWTabButton *button, QWidget *page, bool isSettings) {
    button->setExpanded(m_isExpanded); // 设置按钮的初始展开状态
    m_allButtons.append(button);

    // 将页面添加到 QStackedWidget，并获取其索引作为按钮的 ID
    int pageIndex = m_contentFrame->addWidget(page);
    m_buttonGroup->addButton(button, pageIndex);

    if (isSettings) {
        // 如果是设置项，添加到页脚容器
        auto *layout = static_cast<QVBoxLayout*>(m_footerContainer->layout());
        layout->addWidget(button);
    } else {
        // 否则，添加到主体容器的弹簧之前，以保持靠上排列
        auto *layout = static_cast<QVBoxLayout*>(m_bodyContainer->layout());
        layout->insertWidget(layout->count() - 1, button);
    }

    // 默认选中第一个添加的按钮
    if (m_buttonGroup->buttons().count() == 1) {
        button->setChecked(true);
        _onTabClicked(pageIndex);
    }

    // 为新按钮应用当前主题
    _updateTheme();
}

QWidget* QWNavigationView::headerWidget() const { return m_headerContainer; }
QWidget* QWNavigationView::footerWidget() const { return m_customFooterContainer; }

bool QWNavigationView::isExpanded() const { return m_isExpanded; }
int QWNavigationView::expandedWidth() const { return m_expandedWidth; }
int QWNavigationView::collapsedWidth() const { return m_collapsedWidth; }

void QWNavigationView::setExpanded(bool expanded) {
    if (m_isExpanded == expanded) return;
    m_isExpanded = expanded;

    m_animation->stop(); // 停止任何正在进行的动画
    m_animation->setStartValue(m_navPane->width());
    m_animation->setEndValue(m_isExpanded ? m_expandedWidth : m_collapsedWidth);

    // 更新所有按钮的展开状态
    for (QWTabButton *button : qAsConst(m_allButtons)) {
        button->setExpanded(m_isExpanded);
    }

    m_animation->start();
}

void QWNavigationView::toggleExpansion() {
    setExpanded(!m_isExpanded);
}

void QWNavigationView::setExpandedWidth(int width) {
    m_expandedWidth = width;
    if (m_isExpanded) {
        m_navPane->setFixedWidth(m_expandedWidth);
    }
}

void QWNavigationView::setCollapsedWidth(int width) {
    m_collapsedWidth = width;
    if (!m_isExpanded) {
        m_navPane->setFixedWidth(m_collapsedWidth);
    }
}

void QWNavigationView::_onTabClicked(int id) {
    if (id >= 0 && id < m_contentFrame->count()) {
        m_contentFrame->setCurrentIndex(id);
        emit currentPageChanged(id);
    }
}

void QWNavigationView::_onThemeChanged(bool isDark) {
    Q_UNUSED(isDark);
    _updateTheme();
}

void QWNavigationView::_updateTheme() {
    // 动态地向上查找父窗口，直到找到 QWWindow
    auto *win = qobject_cast<QWWindow*>(window());
    if (!win) {
        // 如果找不到 QWWindow，则不执行任何操作，防止在测试或独立使用时崩溃
        return;
    }

    const QWPalette &palette = win->colorPalette();
    bool isDark = win->isDarkMode();

    // 更新所有已创建按钮的主题
    for (QWTabButton *button : qAsConst(m_allButtons)) {
        button->updateTheme(palette, isDark);
    }
}

void QWNavigationView::paintEvent(QPaintEvent *event) {
    // 将背景绘制为透明，以显示父窗口 QWWindow 的材质效果
    QPainter painter(this);
    painter.fillRect(rect(), Qt::transparent);
    QWidget::paintEvent(event);
}

void QWNavigationView::showEvent(QShowEvent *event) {
    // 确保在控件第一次显示时，立即应用正确的主题
    if (!event->spontaneous()) {
        _updateTheme();
    }
    QWidget::showEvent(event);
}

} // namespace QtWin
