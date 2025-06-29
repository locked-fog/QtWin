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
    
    // 初始化调色板
    setupPalettes();
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

const QWPalette& QWWindow::colorPalette() const {
    return m_palette;
}

QColor QWWindow::getThemeColor(QWPalette::QWColor colorRole, int toneOverride) const {
    int tone = (toneOverride == -1) ? currentTone() : toneOverride;
    return m_palette.getQColor(colorRole, tone);
}

int QWWindow::currentTone() const {
    return m_isDarkMode ? m_darkTone : m_lightTone;
}

bool QWWindow::isDarkMode() const {
    return m_isDarkMode;
}

void QWWindow::setSeedColor(const QColor &color) {
    if (seedColor() != color) {
        m_palette.setSeedColor(RGBColor(color));
        setupPalettes(); // 重新设置调色板
        updateCustomTheme();
        emit seedColorChanged(color);
    }
}

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
        setupPalettes(); // 重新设置调色板
        updateCustomTheme();
        emit toneChanged();
    }
}

void QWWindow::setDarkTone(int tone) {
    if (m_darkTone != tone) {
        m_darkTone = tone;
        setupPalettes(); // 重新设置调色板
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

void QWWindow::setupPalettes() {
    QPalette windowPalette = palette();
    
    // 为窗口设置主要的色彩角色
    QColor windowBg = getThemeColor(QWPalette::neutralAccent);
    QColor windowText = getThemeColor(QWPalette::neutralAccent, m_isDarkMode ? m_lightTone : m_darkTone);
    QColor base = getThemeColor(QWPalette::neutralColor);
    QColor alternateBase = getThemeColor(QWPalette::neutralColor, currentTone() + (m_isDarkMode ? 5 : -5));
    
    // 设置窗口背景色
    windowPalette.setColor(QPalette::Window, windowBg);
    windowPalette.setColor(QPalette::WindowText, windowText);
    
    // 设置输入控件背景色
    windowPalette.setColor(QPalette::Base, base);
    windowPalette.setColor(QPalette::AlternateBase, alternateBase);
    windowPalette.setColor(QPalette::Text, windowText);
    
    // 设置按钮相关色彩
    QColor buttonBg = getThemeColor(QWPalette::subColor);
    QColor buttonText = getThemeColor(QWPalette::subColor, m_isDarkMode ? m_lightTone : m_darkTone);
    windowPalette.setColor(QPalette::Button, buttonBg);
    windowPalette.setColor(QPalette::ButtonText, buttonText);
    
    // 设置高亮色彩
    QColor highlight = getThemeColor(QWPalette::accentColor);
    QColor highlightText = getThemeColor(QWPalette::accentColor, m_isDarkMode ? m_lightTone : m_darkTone);
    windowPalette.setColor(QPalette::Highlight, highlight);
    windowPalette.setColor(QPalette::HighlightedText, highlightText);
    
    // 设置链接色彩
    QColor link = getThemeColor(QWPalette::mainColor);
    QColor visitedLink = getThemeColor(QWPalette::subColor);
    windowPalette.setColor(QPalette::Link, link);
    windowPalette.setColor(QPalette::LinkVisited, visitedLink);
    
    // 应用调色板到窗口
    setPalette(windowPalette);
    
    // 如果有中心控件，也为其设置调色板
    if (m_centralWidget) {
        m_centralWidget->setPalette(windowPalette);
    }
}

void QWWindow::updateCustomTheme() {
    if (m_material != Default) {
        // 材质模式：清除样式表，让透明背景生效
        setStyleSheet("");
        if (m_centralWidget) {
            m_centralWidget->setStyleSheet("");
        }
    } else {
        // Default模式：也清除样式表，完全依赖QPalette系统
        setStyleSheet("");
        if (m_centralWidget) {
            m_centralWidget->setStyleSheet("");
        }
    }
    
    // 重新设置调色板以确保颜色更新
    setupPalettes();
    
    // 触发重绘
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
    
    // 发出主题变化信号
    emit themeChanged(isDark);
}

void QWWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (m_firstShow && windowHandle()) {
        updateFrame(); 
        onThemeChanged(m_isDarkMode);
        m_firstShow = false;
    }
}

void QWWindow::paintEvent(QPaintEvent *event) {
    if (m_material != Default) {
        // 材质模式：手动绘制透明背景
        QPainter painter(this);
        painter.fillRect(rect(), Qt::transparent);
    } else {
        // Default模式：使用标准的Qt绘制流程，依赖QPalette
        QWidget::paintEvent(event);
    }
}

} // namespace QtWin