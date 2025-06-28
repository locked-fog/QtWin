# QtWin 窗口系统 (QWWindow) 开发手册

## 1. 概述

`QWWindow` 是 `QtWin` 控件库的核心容器，旨在完全替代 `QWidget` 或 `QMainWindow` 作为应用的顶层窗口。它提供了与 Windows 11 原生窗口高度一致的外观和行为，并深度集成了 `QtWin` 的色彩和主题系统。

**核心设计理念：**

* **原生体验**：通过直接利用 Windows DWM (桌面窗口管理器) API，`QWWindow` 实现了无边框窗口下的原生圆角、阴影、拖动和缩放，效果流畅自然，无闪烁问题。
* **现代材质**：支持一键切换云母 (Mica) 和亚克力 (Acrylic) 等现代窗口背景材质，赋予应用精致的视觉层次感。
* **色彩驱动**：与 `QWPalette` 无缝集成。开发者只需设定一个种子颜色和两种亮度值（light/dark tone），窗口及其所有子控件的颜色便可自动生成和管理。
* **简洁设计**：遵循类似 Windows 11 “设置”应用的现代设计语言，不提供传统的菜单栏和可扩展的标题栏，专注于内容呈现。
* **主题感知**：自动响应 `QWApplication` 的全局深色/浅色模式切换，实现应用主题的即时同步。

-----

## 2. 如何使用

### 步骤 1: 集成到项目中

使用 `QWWindow` 作为你的主窗口非常简单。

**`main.cpp` 示例：**

```cpp
#include "QtWin/QWApplication.h"
#include "QtWin/QWWindow.h"
#include "QtWin/QWButton.h" // 引入一个QtWin控件作为示例
#include <QLabel>

int main(int argc, char *argv[])
{
    // 1. 实例化 QWApplication
    QtWin::QWApplication app(argc, argv, "MyCompany", "MyApp", "1.0.0");
    app.setDarkMode(false); // 设置初始主题

    // 2. 实例化 QWWindow 作为主窗口
    QtWin::QWWindow window;
    window.setWindowTitle("My Awesome App");
    window.setWindowIcon(QIcon(":/path/to/icon.ico"));
    
    // 3. (可选) 设置颜色和材质
    window.setSeedColor(QColor("#0078D4")); // 设置主题色
    window.setMaterial(QtWin::QWWindow::Mica); // 开启云母材质

    // 4. 创建并设置中心控件
    QWidget* myContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(myContent);
    layout->addWidget(new QLabel("Hello, QtWin!", myContent));
    layout->addWidget(new QtWin::QWButton("Click Me", myContent));
    
    window.setCentralWidget(myContent);

    // 5. 设置初始尺寸并显示窗口
    window.resize(800, 600);
    window.show();

    return app.exec();
}
````

### 步骤 2: 自定义外观

你可以通过 `QWWindow` 提供的接口轻松调整窗口的外观。

#### 设置材质

在运行时切换窗口的背景材质。

```cpp
// 切换到亚克力材质
myWindow->setMaterial(QtWin::QWWindow::Acrylic);

// 恢复为不透明的纯色背景
// 注意：纯色背景的颜色由 QWPalette 根据当前主题自动决定
myWindow->setMaterial(QtWin::QWWindow::Default);
```

| `MaterialType` | 效果 | 平台 |
| :--- | :--- | :--- |
| `Default` | 纯色背景，由 `QWPalette` 控制 | 所有平台 |
| `Mica` | 云母材质，模糊化桌面背景 | Windows 11 |
| `Acrylic` | 亚克力材质，模糊化窗口后的内容 | Windows 11 |

#### 定制颜色方案

`QWWindow` 的颜色由 `QWPalette` 驱动。你可以通过以下方式影响颜色。

```cpp
// 1. 改变整个应用的主题色
myWindow->setSeedColor(QColor("purple"));

// 2. 微调浅色模式下的背景/前景亮度
// Tone 值范围 0 (黑) - 100 (白)
myWindow->setLightTone(95); // 更亮的背景

// 3. 微调深色模式下的背景/前景亮度
myWindow->setDarkTone(15); // 更深的背景
```

窗口会自动监听 `QWApplication::instance()->setDarkMode()` 的调用，并根据 `lightTone` 和 `darkTone` 的值在两种模式间切换。

-----

## 3\. QWWindow API 参考

### 属性

| 属性 | 类型 | 描述 |
| :--- | :--- | :--- |
| `seedColor` | `QColor` | 设置驱动 `QWPalette` 的种子颜色，影响所有控件的主题色。 |
| `material` | `MaterialType` | 设置窗口的背景材质，如 Mica、Acrylic。 |
| `lightTone` | `int` | 设置浅色模式下背景的亮度值 (0-100)。前景会自动计算。 |
| `darkTone` | `int` | 设置深色模式下背景的亮度值 (0-100)。前景会自动计算。 |

### 公共函数

| 函数 | 返回 | 描述 |
| :--- | :--- | :--- |
| `setCentralWidget(QWidget*)` | `void` | 设置窗口中心的内容控件。窗口会接管其所有权。 |
| `centralWidget()` | `QWidget*`| 获取当前的中心控件。 |
