#include "QtWin/QWWindow.h"
#include "QtWin/QWApplication.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include <QIcon>
#include <QPushButton>
#include <QShowEvent>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif
#endif

namespace QtWin {

class QWWindow::TitleBar : public QWidget {
public:
    explicit TitleBar(QWidget *parent = nullptr) : QWidget(parent) {
        m_iconLabel = new QLabel(this);
        m_titleLabel = new QLabel(this);
        m_minimizeButton = new QPushButton("—", this);
        m_maximizeButton = new QPushButton("☐", this);
        m_closeButton = new QPushButton("✕", this);
        m_iconLabel->setFixedSize(16, 16);
        m_iconLabel->setScaledContents(true);
        m_titleLabel->setObjectName("titleLabel");
        auto styleButtons = [](QPushButton* btn) {
            btn->setFixedSize(46, 32);
            btn->setFlat(true);
            btn->setFocusPolicy(Qt::NoFocus);
        };
        styleButtons(m_minimizeButton);
        styleButtons(m_maximizeButton);
        styleButtons(m_closeButton);
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(8, 0, 0, 0);
        layout->setSpacing(8);
        layout->addWidget(m_iconLabel);
        layout->addWidget(m_titleLabel);
        layout->addStretch();
        layout->addWidget(m_minimizeButton);
        layout->addWidget(m_maximizeButton);
        layout->addWidget(m_closeButton);
        setLayout(layout);
        setFixedHeight(32);
        connect(m_minimizeButton, &QPushButton::clicked, parent, &QWidget::showMinimized);
        connect(m_maximizeButton, &QPushButton::clicked, this, [parent] {
            parent->isMaximized() ? parent->showNormal() : parent->showMaximized();
        });
        connect(m_closeButton, &QPushButton::clicked, parent, &QWidget::close);
    }
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;
};

QWWindow::QWWindow(QWidget *parent)
    : QWidget(parent, Qt::Window),
      m_titleBar(nullptr),
      m_rootLayout(nullptr),
      m_contentArea(nullptr),
      m_centralWidget(nullptr),
      m_lightTone(90),
      m_darkTone(10),
      m_isDarkMode(false),
      m_material(Mica)
{
    initialize();
}

QWWindow::~QWWindow() = default;

void QWWindow::initialize() {
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    m_titleBar = new TitleBar(this);
    m_rootLayout = new QVBoxLayout(this);
    m_rootLayout->setContentsMargins(0, 0, 0, 0);
    m_rootLayout->setSpacing(0);
    m_rootLayout->addWidget(m_titleBar);
    
    m_contentArea = new QWidget(this);
    m_contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_rootLayout->addWidget(m_contentArea, 1);

    setLayout(m_rootLayout);

    connect(this, &QWidget::windowTitleChanged, this, &QWWindow::onWindowTitleChanged);
    connect(this, &QWidget::windowIconChanged, this, [this](const QIcon &icon){
        m_titleBar->m_iconLabel->setPixmap(icon.pixmap(16, 16));
    });

    if (QWApplication::instance()) {
        m_isDarkMode = QWApplication::instance()->isDarkMode();
        connect(QWApplication::instance(), &QWApplication::darkModeChanged, this, &QWWindow::onThemeChanged);
    }
    
    resize(800, 600);
}

void QWWindow::setCentralWidget(QWidget *widget) {
    if (m_centralWidget == widget) return;

    if (m_contentArea) {
        m_rootLayout->removeWidget(m_contentArea);
        m_contentArea->deleteLater();
        m_contentArea = nullptr;
    }
    if (m_centralWidget) {
        m_rootLayout->removeWidget(m_centralWidget);
        m_centralWidget->deleteLater();
        m_centralWidget = nullptr;
    }
    
    if(widget){
        m_centralWidget = widget;
        m_rootLayout->addWidget(widget, 1);
        widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    } else {
        m_contentArea = new QWidget(this);
        m_contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_rootLayout->addWidget(m_contentArea, 1);
    }
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

void QWWindow::setMaterial(QWWindow::MaterialType type) {
    if (m_material != type) {
        m_material = type;
        updateCustomTheme();
        updateFrame();
        emit materialChanged(type);
    }
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

    DWM_WINDOW_CORNER_PREFERENCE cornerPreference = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPreference, sizeof(cornerPreference));
    
    BOOL isDark = m_isDarkMode;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &isDark, sizeof(isDark));
    
    auto backdropType = static_cast<DWM_SYSTEMBACKDROP_TYPE>(m_material);
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
#endif
    update();
}

void QWWindow::updateCustomTheme() {
    int currentTone = m_isDarkMode ? m_darkTone : m_lightTone;
    int textTone = m_isDarkMode ? m_lightTone : m_darkTone;
    QColor windowBgColor = m_palette.getQColor(QWPalette::neutralColor, currentTone);
    QColor textColor = m_palette.getQColor(QWPalette::neutralColor, textTone);
    QColor palColor = (m_material != Default) ? Qt::transparent : windowBgColor;
    
    QPalette p = palette();
    p.setColor(QPalette::Window, palColor);
    p.setColor(QPalette::WindowText, textColor);
    setPalette(p);
    
    m_titleBar->m_titleLabel->setStyleSheet(QString("QLabel#titleLabel { color: %1; }").arg(textColor.name()));
    QColor hoverColor = m_palette.getQColor(QWPalette::neutralColor, m_isDarkMode ? currentTone + 10 : currentTone - 10);
    QColor pressedColor = m_palette.getQColor(QWPalette::neutralColor, m_isDarkMode ? currentTone + 20 : currentTone - 20);
    QString buttonStyle = QString("QPushButton { border: none; color: %1; } QPushButton:hover { background-color: %2; } QPushButton:pressed { background-color: %3; }").arg(textColor.name(), hoverColor.name(), pressedColor.name());
    m_titleBar->m_minimizeButton->setStyleSheet(buttonStyle);
    m_titleBar->m_maximizeButton->setStyleSheet(buttonStyle);
    m_titleBar->m_closeButton->setStyleSheet(QString("QPushButton { border: none; color: %1; } QPushButton:hover { background-color: #C42B1C; color: white; } QPushButton:pressed { background-color: #A32416; color: white; }").arg(textColor.name()));

    QWidget* content = m_centralWidget ? m_centralWidget : m_contentArea;
    if (content) {
        content->setAutoFillBackground(true);
        QPalette centralPalette = content->palette();
        centralPalette.setColor(QPalette::Window, palColor);
        centralPalette.setColor(QPalette::WindowText, textColor);
        content->setPalette(centralPalette);
    }
    update();
}

void QWWindow::onThemeChanged(bool isDark) {
    if (m_isDarkMode != isDark) {
        m_isDarkMode = isDark;
        updateCustomTheme();
        updateFrame();
    }
}

void QWWindow::onWindowTitleChanged(const QString &title) { m_titleBar->m_titleLabel->setText(title); }

void QWWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    static bool isFirstShow = true;
    if (isFirstShow && windowHandle()) {
        updateFrame();
        updateCustomTheme();
        isFirstShow = false;
    }
}

void QWWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMaximized) {
            m_titleBar->m_maximizeButton->setText("🗗");
        } else {
            m_titleBar->m_maximizeButton->setText("☐");
        }
        m_titleBar->update();
    }
    QWidget::changeEvent(event);
}

void QWWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (m_material != Default) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(event->rect(), Qt::transparent);
    } else {
        painter.fillRect(event->rect(), palette().color(QPalette::Window));
    }
}

bool QWWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        switch (msg->message) {
            case WM_NCCALCSIZE: {
                if (msg->wParam == TRUE) {
                    *result = WVR_REDRAW;
                    return true;
                }
                *result = 0;
                return true;
            }
            case WM_NCHITTEST: {
                *result = 0;
                const LONG border_width = 8;
                RECT winrect;
                GetWindowRect(msg->hwnd, &winrect);

                long x = GET_X_LPARAM(msg->lParam);
                long y = GET_Y_LPARAM(msg->lParam);
                
                if (!isMaximized() && !isFullScreen()) {
                    bool onLeft = x >= winrect.left && x < winrect.left + border_width;
                    bool onRight = x < winrect.right && x >= winrect.right - border_width;
                    bool onTop = y >= winrect.top && y < winrect.top + border_width;
                    bool onBottom = y < winrect.bottom && y >= winrect.bottom - border_width;

                    if (onTop && onLeft) *result = HTTOPLEFT;
                    else if (onTop && onRight) *result = HTTOPRIGHT;
                    else if (onBottom && onLeft) *result = HTBOTTOMLEFT;
                    else if (onBottom && onRight) *result = HTBOTTOMRIGHT;
                    else if (onLeft) *result = HTLEFT;
                    else if (onRight) *result = HTRIGHT;
                    else if (onTop) *result = HTTOP;
                    else if (onBottom) *result = HTBOTTOM;
                }
                
                if (*result != 0) return true;

                QPoint localMousePos = mapFromGlobal(QPoint(x, y));
                if (m_titleBar->geometry().contains(localMousePos) && !m_titleBar->childAt(localMousePos)) {
                     *result = HTCAPTION;
                     return true;
                }
                
                *result = HTCLIENT;
                return true;
            }
        }
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

} // namespace QtWin