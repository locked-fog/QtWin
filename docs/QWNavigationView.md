# QtWin 导航视图 (QWNavigationView) 开发手册

## 1. 概述

`QWNavigationView` 是 `QtWin` 库的核心导航控件，其设计灵感来源于 Microsoft 的 WinUI 3 `NavigationView`。它旨在为桌面应用程序提供现代化、多功能且易于使用的导航框架。

作为应用的中心骨架，`QWNavigationView` 通常被设置在 `QWWindow` 内部，它能够自动适应父窗口的主题（深色/浅色模式）和材质（云母/亚克力），为用户提供统一且沉浸的视觉体验。

**核心设计理念：**

* **结构化布局**: 左侧为可折叠的导航面板，右侧为内容显示区域。导航面板本身分为页眉、主体和页脚三个逻辑区域，便于组织不同类型的导航项。
* **灵活的定制性**: 通过 `headerWidget()` 和 `footerWidget()` 方法，开发者可以完全控制页眉和页脚区域的内容，轻松添加标题、折叠按钮、搜索框或版权信息等自定义控件。
* **流畅的交互**: 内置平滑的展开/收起动画 (`QPropertyAnimation`)，提供符合现代 UI 设计标准的动态交互体验。
* **主题自适应**: 无需手动配置，控件会自动从父级 `QWWindow` 获取 `QWPalette` 实例，并监听 `QWApplication` 的主题变化信号，以实时更新所有元素的颜色。

## 2. 如何使用

### 步骤 1: 创建并设置 QWNavigationView

在您的主窗口中，创建 `QWNavigationView` 实例，并将其设置为 `QWWindow` 的中心控件。

```cpp
// main.cpp 或 mainwindow.cpp
#include "QtWin/QWApplication.h"
#include "QtWin/QWWindow.h"
#include "QtWin/QWNavigationView.h"

// 为了方便演示，我们还需要一些其他控件
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon> // 用于设置图标

int main(int argc, char *argv[])
{
    QtWin::QWApplication app(argc, argv, "MyCompany", "MyApp", "1.0.0");

    QtWin::QWWindow window;
    window.setWindowTitle("My Awesome App");
    window.setSeedColor(QColor(19, 149, 192)); // 设置主题色

    // 1. 创建 QWNavigationView 实例
    auto *navView = new QtWin::QWNavigationView(&window);

    // 2. 将其设置为 QWWindow 的中心控件
    window.setCentralWidget(navView);

    // ... 后续步骤将在这里添加 ...

    window.resize(900, 700);
    window.show();

    return app.exec();
}
````

### 步骤 2: 添加页面和配置页眉

使用 `addPage()` 和 `addSettingsPage()` 添加导航项，并使用 `headerWidget()` 定制页眉。

```cpp
// ...接上文...

// 3. 添加主导航页面
// 每个页面可以是任何 QWidget，这里我们用 QLabel 作为示例
navView->addPage(new QLabel("这是主页", navView), QIcon(":/icons/home.svg"), "主页");
navView->addPage(new QLabel("这是数据分析页面", navView), QIcon(":/icons/data.svg"), "数据分析");
navView->addPage(new QLabel("这是邮件页面", navView), QIcon(":/icons/mail.svg"), "邮件");

// 4. 添加一个设置页面，它会自动出现在导航栏底部
navView->addSettingsPage(new QLabel("这是设置页面", navView), QIcon(":/icons/settings.svg"), "设置");

// 5. 自定义页眉内容
// 获取页眉容器
QWidget* header = navView->headerWidget();
auto* headerLayout = new QHBoxLayout(header);
headerLayout->setContentsMargins(12, 0, 12, 0);

// 添加一个折叠按钮
auto* collapseButton = new QPushButton(QIcon(":/icons/menu.svg"), "", header);
collapseButton->setFixedSize(32, 32);
collapseButton->setFlat(true); // 设置为扁平样式

// 添加一个标题
auto* titleLabel = new QLabel("My App", header);
titleLabel->setStyleSheet("font-size: 14px; font-weight: bold;");

headerLayout->addWidget(collapseButton);
headerLayout->addWidget(titleLabel);
headerLayout->addStretch();

// 连接折叠按钮的点击信号到 navView 的槽函数
QObject::connect(collapseButton, &QPushButton::clicked, navView, &QtWin::QWNavigationView::toggleExpansion);

// ...接上文...
```

## 3\. API 参考

### 主要公共函数

| 函数                                                         | 描述                                                 |
| :----------------------------------------------------------- | :--------------------------------------------------- |
| `void addPage(QWidget*, const QIcon&, const QString&)`       | 添加一个主导航项及其对应的页面。                     |
| `void addSettingsPage(QWidget*, const QIcon&, const QString&)` | 添加一个固定在底部的设置/页脚导航项。                |
| `QWidget* headerWidget() const`                              | 获取页眉容器的指针，允许向其中添加自定义控件。       |
| `QWidget* footerWidget() const`                              | 获取页脚容器的指针，用于在设置项下方添加内容。       |
| `void toggleExpansion()`                                     | 切换导航面板的展开/收起状态（带动画）。              |

### 属性

| 属性           | 类型   | 描述                                       |
| :------------- | :----- | :----------------------------------------- |
| `isExpanded`   | `bool` | 获取或设置导航面板的展开状态。             |
| `expandedWidth`  | `int`  | 获取或设置导航面板展开时的宽度（像素）。   |
| `collapsedWidth` | `int`  | 获取或设置导航面板收起时的宽度（像素）。   |

### 信号

| 信号                             | 描述                                       |
| :------------------------------- | :----------------------------------------- |
| `void currentPageChanged(int index)` | 当用户切换页面时发射，参数为新页面的索引。 |

## 4\. 核心实现简介

  * **内部布局**: `QWNavigationView` 的主体是一个水平布局，左侧是导航面板 `m_navPane`，右侧是内容区域 `m_contentFrame` (`QStackedWidget`)。
  * **导航按钮**: 所有导航项均由内部的 `QWTabButton` 类实现。此类负责自身的绘制，包括处理选中、悬停、展开/收起时的不同视觉效果。
  * **状态管理**:
      * 导航项的互斥选择由 `QButtonGroup` 自动管理，简化了逻辑。
      * 展开/收起状态由一个布尔成员变量 `m_isExpanded` 控制，所有相关的视觉变化都基于此状态。
  * **动画效果**: 导航面板的宽度变化通过 `QPropertyAnimation` 实现，动画目标是 `m_navPane` 的 `minimumWidth` 属性，确保了布局的平滑过渡。
  * **主题系统**: 控件在 `showEvent` 和 `_onThemeChanged` 槽函数中，通过 `qobject_cast<QWWindow*>(window())` 向上查找父窗口并获取 `QWPalette` 实例，从而保证颜色与全局主题同步。