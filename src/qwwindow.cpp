#include "QtWin/QWWindow.h"
#include "QtWin/QWApplication.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QShowEvent>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <Uxtheme.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif
#endif

namespace QtWin {

QWWindow::QWWindow(QWidget *parent)
    : QWidget(parent),
        m_rootLayout(nullptr),
        m_centralWidget(nullptr),
        m_lightTone(80),
        m_darkTone(20),
        m_isDarkMode(false), // 初始值会被initialize覆盖
        m_material(Acrylic),
        m_firstShow(true)
{
    initialize();
}

QWWindow::~QWWindow() = default;

// 【新增】QSS样式生成函数
QString QWWindow::generateColorStyleSheet(const QColor& color) const {
    return QString("background-color: rgb(%1, %2, %3);")
        .arg(color.red()).arg(color.green()).arg(color.blue());
}

void QWWindow::initialize() {
    m_rootLayout = new QVBoxLayout(this);
    m_rootLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_rootLayout);

    if (QWApplication::instance()) {
        m_isDarkMode = QWApplication::instance()->isDarkMode();
        connect(QWApplication::instance(), &QWApplication::darkModeChanged, this, &QWWindow::onThemeChanged);
    }
    
    m_palette.setSeedColor(QColor(19, 149, 192));
    resize(800, 600);
}

void QWWindow::setCentralWidget(QWidget *widget) {
    if (m_centralWidget == widget) return;

    if (m_centralWidget) {
        m_rootLayout->removeWidget(m_centralWidget);
        m_centralWidget->deleteLater();
    }
    
    m_centralWidget = widget;

    if (m_centralWidget) {
        m_rootLayout->addWidget(m_centralWidget, 1);
        m_centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
    
    // 每当设置新的中心控件时，立即应用当前的主题和样式
    updateCustomTheme(); 
}


QWidget* QWWindow::centralWidget() const { return m_centralWidget; }
QColor QWWindow::seedColor() const { return m_palette.getQColor(QWPalette::mainColor, 50); }
QWWindow::MaterialType QWWindow::material() const { return m_material; }
int QWWindow::lightTone() const { return m_lightTone; }
int QWWindow::darkTone() const { return m_darkTone; }

void QWWindow::setSeedColor(const QColor &color) {
    if (seedColor() != color) {
        m_palette.setSeedColor(RGBColor(color));
        updateCustomTheme();
        emit seedColorChanged(color);
    }
}

// setMaterial现在只负责更新状态，然后委托给updateCustomTheme来应用视觉变化
void QWWindow::setMaterial(QWWindow::MaterialType type) {
    if (m_material == type) return;
    m_material = type;

    // 更新DWM框架，应用或移除材质
    updateFrame();
    // 更新Qt部分的样式
    updateCustomTheme();

    emit materialChanged(type);
}

void QWWindow::setLightTone(int tone) {
    if (m_lightTone != tone) {
        m_lightTone = tone;
        updateCustomTheme();
        emit toneChanged();
    }
}

void QWWindow::setDarkTone(int tone) {
    if (m_darkTone != tone) {
        m_darkTone = tone;
        updateCustomTheme();
        emit toneChanged();
    }
}

void QWWindow::updateFrame() {
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    if (!IsWindow(hwnd)) return;

    bool material_is_active = (m_material != Default);
    
    // 【关键】无论何种情况，都必须设置WA_TranslucentBackground，因为DwmExtendFrameIntoClientArea需要它
    setAttribute(Qt::WA_TranslucentBackground, material_is_active);

    if (material_is_active) {
        auto backdropType = static_cast<int>(m_material);
        DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        MARGINS margins = {-1};
        DwmExtendFrameIntoClientArea(hwnd, &margins);
    } else {
        int backdropType = DWMSBT_NONE; // DWMSBT_AUTO
        DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        MARGINS margins = {0, 0, 0, 0};
        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }
#endif
}

// 【核心】updateCustomTheme现在是所有样式控制的中心
void QWWindow::updateCustomTheme() {
    QString styleSheetString;

    if (m_material != Default) {
        // 材质模式：强制背景透明
        styleSheetString = "background: transparent;";
    } else {
        // Default模式：从调色板计算颜色并生成QSS
        int currentTone = m_isDarkMode ? m_darkTone : m_lightTone;
        QColor windowBgColor = m_palette.getQColor(QWPalette::neutralAccent, currentTone);
        styleSheetString = generateColorStyleSheet(windowBgColor);
    }

    // 将生成的样式表应用到 QWWindow 自身和 centralWidget
    this->setStyleSheet(styleSheetString);
    if (m_centralWidget) {
        m_centralWidget->setStyleSheet(styleSheetString);
    }
    
    // 【可选但推荐】我们依然可以更新文本调色板，因为它不受QSS背景设置影响
    if (m_centralWidget) {
        int textTone = m_isDarkMode ? m_lightTone : m_darkTone;
        QColor textColor = m_palette.getQColor(QWPalette::neutralAccent, textTone);
        QPalette p = m_centralWidget->palette();
        p.setColor(QPalette::WindowText, textColor);
        p.setColor(QPalette::Text, textColor);
        m_centralWidget->setPalette(p);
    }
    
    // 触发重绘以应用所有样式
    update();
}

void QWWindow::onThemeChanged(bool isDark) {
    if (m_isDarkMode == isDark) return;
    m_isDarkMode = isDark;

// #ifdef Q_OS_WIN
//     // 更新标题栏颜色
//     HWND hwnd = reinterpret_cast<HWND>(this->winId());
//     if (IsWindow(hwnd)) {
//         BOOL isDarkBool = m_isDarkMode;
//         DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &isDarkBool, sizeof(isDarkBool));
//     }
// #endif

    // 更新内部控件的主题
    updateCustomTheme();
}

void QWWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (m_firstShow && windowHandle()) {
        updateFrame(); 
        onThemeChanged(m_isDarkMode);
        m_firstShow = false;
    }
}

// paintEvent 的职责现在非常纯粹
void QWWindow::paintEvent(QPaintEvent *event) {
    // 只有在启用DWM材质时，才需要手动绘制透明背景
    if (m_material != Default) {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::transparent);
    } else {
        // 在Default模式下，我们什么都不做，让Qt根据我们设置的QSS来自动绘制背景
        QWidget::paintEvent(event);
    }
}

} // namespace QtWin