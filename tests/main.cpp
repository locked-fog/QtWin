/**
 * @file main.cpp
 * @brief QtWin 库功能测试程序
 *
 * 这个文件包含一个单一的 main 函数，用于全面测试和展示 QtWin 库的各项核心功能。
 * 它创建了一个现代化的窗口，其中包含一个可折叠的导航视图和一系列用于动态交互的测试控件。
 *
 * 测试的功能点包括：
 * - QWApplication: 应用生命周期管理、主题切换。
 * - QWWindow: 窗口容器、材质背景 (Mica/Acrylic)、动态主题色。
 * - QWLogger: 日志记录。
 * - QWSettings: 窗口状态的持久化存储与恢复。
 * - QWNavigationView: 核心导航控件，包括页面管理、页眉/页脚定制和动画效果。
 * - QWTabButton: (间接测试) 作为 QWNavigationView 的一部分被使用和展示。
 */

// --- Qt Headers ---
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QColorDialog>
#include <QIcon>
#include <QApplication> // Fallback for static analysis if needed

// --- QtWin Library Headers ---
#include <QtWin/QWApplication.h>
#include <QtWin/QWWindow.h>
#include <QtWin/QWNavigationView.h>
#include <QtWin/QWLogger.h>
#include <QtWin/QWPalette.h>
#include <QtWin/QWSettings.h>



// 主函数入口
int main(int argc, char *argv[])
{
    // --- 1. 初始化 QWApplication ---
    // QWApplication 是 QtWin 应用的入口点，它会自动处理日志和设置的初始化。
    // "QtWin" 是组织名，"TestApp" 是应用名，这对 QSettings 定位配置文件路径至关重要。
    QtWin::QWApplication app(argc, argv, "QtWin", "TestApp", "1.0.0");
    qwLogger(QtWin::LogLevel::Info, logGeneral) << "测试程序启动。";


    // --- 2. 创建主窗口 (TestWindow) ---
    // 使用我们自定义的 TestWindow 类，以便处理关闭事件。
    QtWin::QWWindow window;
    window.setWindowTitle("QtWin Library Test Application");
    window.setWindowIcon(QIcon(":/icons/app.ico")); // 假设有一个应用图标


    // --- 3. 创建核心导航控件 QWNavigationView ---
    auto *navView = new QtWin::QWNavigationView(&window);


    // --- 4. 创建测试页面 ---
    // 主页：将包含所有用于动态测试的控件
    auto* homePage = new QWidget(navView);
    auto* homeLayout = new QVBoxLayout(homePage);
    homeLayout->setContentsMargins(20, 20, 20, 20);
    homeLayout->setSpacing(15);
    homeLayout->setAlignment(Qt::AlignTop);

    // 其他页面：仅用 QLabel 作为占位符
    auto* dataPage = new QLabel("数据分析页面\n\n在这里展示图表和数据。", navView);
    dataPage->setAlignment(Qt::AlignCenter);
    auto* mailPage = new QLabel("邮件页面\n\n在这里展示邮件列表。", navView);
    mailPage->setAlignment(Qt::AlignCenter);
    auto* settingsPage = new QLabel("设置页面\n\n在这里放置应用的各项设置选项。", navView);
    settingsPage->setAlignment(Qt::AlignCenter);


    // --- 5. 向 NavigationView 添加页面 ---
    // 使用虚构的图标路径，实际使用时需要创建资源文件
    navView->addPage(homePage, QIcon(), "主页");
    navView->addPage(dataPage, QIcon(), "数据分析");
    navView->addPage(mailPage, QIcon(), "邮件");
    // addSettingsPage 会将页面固定在导航栏底部
    navView->addSettingsPage(settingsPage, QIcon(), "设置");


    // --- 6. 自定义 NavigationView 页眉 ---
    // headerWidget() 返回一个 QWidget 容器，我们可以向其中添加任何控件。
    QWidget* header = navView->headerWidget();
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 0, 12, 0);

    // 创建一个折叠/展开按钮
    auto* collapseButton = new QPushButton("☰", header);
    collapseButton->setFixedSize(32, 32);
    collapseButton->setToolTip("展开/收起导航面板");
    // 将按钮的点击信号连接到 navView 的 toggleExpansion 槽
    QObject::connect(collapseButton, &QPushButton::clicked, navView, &QtWin::QWNavigationView::toggleExpansion);

    auto* titleLabel = new QLabel("QtWin 测试", header);
    titleLabel->setStyleSheet("font-size: 14px;");

    headerLayout->addWidget(collapseButton);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();


    // --- 7. 向主页添加测试控件 ---
    homeLayout->addWidget(new QLabel("<h3>QtWin 动态测试</h3>", homePage));

    // 创建一个水平布局容器用于放置按钮
    auto* buttonContainer = new QWidget(homePage);
    auto* buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0,0,0,0);

    // 主题切换按钮
    auto* themeButton = new QPushButton("切换深色/浅色模式", homePage);
    QObject::connect(themeButton, &QPushButton::clicked, &app, [&]() {
        app.toggleDarkMode();
        qwLogger(QtWin::LogLevel::Info, logGeneral) << "主题已切换为: " << (app.isDarkMode() ? "深色" : "浅色");
    });
    buttonLayout->addWidget(themeButton);

    // 种子颜色切换按钮
    auto* colorButton = new QPushButton("改变主题色", homePage);
    QObject::connect(colorButton, &QPushButton::clicked, &window, [&]() {
        const QColor color = QColorDialog::getColor(window.seedColor(), &window, "选择一个新的主题色");
        if (color.isValid()) {
            window.setSeedColor(color);
            qwLogger(QtWin::LogLevel::Info, logGeneral) << "主题色已改变: " << color.name();
        }
    });
    buttonLayout->addWidget(colorButton);

    // 日志记录按钮
    auto* logButton = new QPushButton("记录一条日志", homePage);
    QObject::connect(logButton, &QPushButton::clicked, []() {
        qwLogger(QtWin::LogLevel::Debug, logGeneral) << "这是一条由按钮点击触发的【Debug】级别测试日志。";
    });
    buttonLayout->addWidget(logButton);

    homeLayout->addWidget(buttonContainer);


    // 材质切换下拉框
    auto* materialComboBox = new QComboBox(homePage);
    materialComboBox->addItem("默认背景 (Default)", static_cast<int>(QtWin::QWWindow::Default));
    materialComboBox->addItem("云母材质 (Mica)", static_cast<int>(QtWin::QWWindow::Mica));
    materialComboBox->addItem("亚克力材质 (Acrylic)", static_cast<int>(QtWin::QWWindow::Acrylic));
    materialComboBox->addItem("Tabbed云母 (MicaTabbed)", static_cast<int>(QtWin::QWWindow::MicaTabbed));

    QObject::connect(materialComboBox, &QComboBox::activated, &window, [&](int index){
        auto material = static_cast<QtWin::QWWindow::MaterialType>(materialComboBox->itemData(index).toInt());
        window.setMaterial(material);
        qwLogger(QtWin::LogLevel::Info, logGeneral) << "窗口材质已切换。";
    });
    homeLayout->addWidget(new QLabel("窗口背景材质:", homePage));
    homeLayout->addWidget(materialComboBox);


    // --- 8. 窗口最终设置与状态恢复 ---
    window.setCentralWidget(navView); // 将导航视图设置为窗口的中心控件

    // 从 QSettings 恢复上次关闭时的状态
    auto* settings = app.settings();
    if (settings->contains("window/geometry")) {
        window.restoreGeometry(settings->value("window/geometry").toByteArray());
        qwLogger(QtWin::LogLevel::Info, logGeneral) << "窗口位置和尺寸已恢复。";
    } else {
        window.resize(900, 700); // 首次启动时设置默认大小
    }
    if (settings->contains("theme/darkMode")) {
        app.setDarkMode(settings->value("theme/darkMode").toBool());
        qwLogger(QtWin::LogLevel::Info, logGeneral) << "主题模式已恢复。";
    }


    // --- 9. 显示窗口并启动事件循环 ---
    window.show();
    return app.exec();
}