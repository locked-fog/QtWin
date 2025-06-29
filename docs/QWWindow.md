# QtWin 窗口系统 (QWWindow) 开发手册

## 1. 概述

`QWWindow` 是 `QtWin` 控件库的核心容器，旨在完全替代 `QWidget` 或 `QMainWindow` 作为应用的顶层窗口。它致力于在保留原生窗口稳定性的前提下，提供与 Windows 11 高度一致的现代外观和行为。

**核心设计理念：**

* **原生体验**：`QWWindow` 使用标准的窗口框架，通过直接利用 Windows DWM (桌面窗口管理器) API，实现了流畅无闪烁的原生窗口拖动、缩放和动画。
* **现代材质**：支持一键切换云母 (Mica)、亚克力 (Acrylic)、MicaTabbed 等现代窗口背景材质。当设置为 `Default` 模式时，它会使用由 `QWPalette` 驱动的纯色背景窗口。
* **色彩管理**：与 `QWPalette` 无缝集成，提供完整的色彩管理系统，包括种子颜色设置、亮/暗色调控制和主题颜色获取。
* **主题感知**：自动响应 `QWApplication` 的全局深色/浅色模式切换，并能通过 DWM API 同步 Windows 系统标题栏的颜色。

## 2. 如何使用

### 步骤 1: 集成到项目中

```cpp
#include "QtWin/QWApplication.h"
#include "QtWin/QWWindow.h"
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QtWin::QWApplication app(argc, argv, "MyCompany", "MyApp", "1.0.0");
    app.setDarkMode(false); // 设置初始为浅色模式

    QtWin::QWWindow window;
    window.setWindowTitle("My Awesome App");
    window.setWindowIcon(QIcon(":/path/to/icon.ico"));
    
    // 设置颜色和材质
    window.setSeedColor(QColor("#0078D4"));   // 设置主题色
    window.setMaterial(QtWin::QWWindow::Mica); // 初始启用云母材质
    window.setLightTone(80); // 浅色模式色调
    window.setDarkTone(20);  // 深色模式色调

    // 创建并设置中心控件
    QWidget* myContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(myContent);
    QLabel* label = new QLabel("Hello, QtWin!", myContent);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    window.setCentralWidget(myContent);
    window.resize(800, 600);
    window.show();

    return app.exec();
}
```

### 步骤 2: 自定义外观

#### 设置材质

```cpp
// 可用材质类型
enum MaterialType {
    Default = 1,    // 纯色背景
    Mica = 2,       // 云母材质
    Acrylic = 3,    // 亚克力材质  
    MicaTabbed = 4  // Tabbed云母材质
};

// 切换到亚克力材质
myWindow->setMaterial(QtWin::QWWindow::Acrylic);
```

#### 颜色管理

```cpp
// 获取调色板引用
const QWPalette& palette = myWindow->colorPalette();

// 获取当前主题颜色
QColor color = myWindow->getThemeColor(QWPalette::mainColor);

// 检查当前主题模式
bool isDark = myWindow->isDarkMode();
int currentTone = myWindow->currentTone();
```

## 3. QWWindow API 参考

### 属性

| 属性        | 类型           | 描述 |
| :---------- | :------------- | :--- |
| `seedColor` | `QColor`       | 设置/获取种子颜色 |
| `material`  | `MaterialType` | 设置/获取窗口材质类型 |
| `lightTone` | `int`          | 设置/获取浅色模式色调值 (0-100) |
| `darkTone`  | `int`          | 设置/获取深色模式色调值 (0-100) |

### 公共函数

| 函数                          | 返回       | 描述 |
| :---------------------------- | :--------- | :--- |
| `setCentralWidget(QWidget*)`  | `void`     | 设置中心控件 |
| `centralWidget()`             | `QWidget*` | 获取中心控件 |
| `colorPalette()`              | `const QWPalette&` | 获取调色板引用 |
| `getThemeColor(QWColor, int)` | `QColor`   | 获取指定角色和色调的颜色 |
| `currentTone()`               | `int`      | 获取当前主题的色调值 |
| `isDarkMode()`                | `bool`     | 检查是否为深色模式 |

## 4. 核心实现

`QWWindow` 的实现结合了 Qt 样式系统和 Windows DWM API：

* **色彩系统**：使用 `QWPalette` 管理所有颜色计算，通过 `setupPalettes()` 方法更新窗口和子控件的调色板。
* **材质实现**：
  - 材质模式下：设置 `WA_TranslucentBackground` 属性并调用 DWM API 扩展窗口框架
  - Default 模式下：使用 `QPalette` 设置纯色背景
* **主题切换**：通过 `onThemeChanged()` 响应系统主题变化，更新所有子控件的颜色
* **绘制优化**：`paintEvent` 在材质模式下仅绘制透明背景，在 Default 模式下委托给基类处理
