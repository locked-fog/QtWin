# QtWin 日志系统 (QWLogger) 开发手册

## 1\. 概述

`QWLogger` 是 `QtWin` 控件库内置的、统一的日志记录系统。它基于 Qt 强大的结构化日志框架构建，旨在提供一个高性能、可配置且易于使用的日志记录方案。

**核心设计理念：**

  * **自动初始化**：开发者无需关心日志系统的初始化。`QWApplication` 在启动时会自动配置好 `QWLogger`。
  * **标准接口**：开发者通过标准的 Qt 日志宏 (`qCInfo`, `qCWarning` 等) 来记录日志，无需学习新的 API。
  * **结构化与分类**：所有日志都应归属到一个**日志类别 (`QLoggingCategory`)** 中，便于对不同模块的日志进行独立过滤和分析。
  * **统一输出**：默认情况下，所有日志消息都会被重定向到一个位于标准应用数据目录下的 `app.log` 文件中，同时也会输出到控制台（调试时）。

-----

## 2\. 如何记录日志

在您的代码中记录日志非常简单。遵循以下三步即可：

### 步骤 1: 声明并定义日志类别

为了对日志进行分类，您需要为您的模块或类创建一个 `QLoggingCategory`。

**在 `.h` 头文件中声明类别：**

```cpp
// mywidget.h
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logMyWidget) // 声明一个名为 logMyWidget 的类别

```

**在 `.cpp` 源文件中定义类别：**

```cpp
// mywidget.cpp
#include "mywidget.h"

// 定义类别，并为其指定一个唯一的、全局的字符串ID
Q_LOGGING_CATEGORY(logMyWidget, "qtwin.ui.mywidget")
```

> **命名约定**: 类别字符串 ID 推荐使用小写字母和点 `.` 进行分层，例如 `qtwin.core.network` 或 `qtwin.ui.dialogs`。

### 步骤 2: 使用分类日志宏记录消息

使用 `qC` 开头的宏，并将您定义的类别名作为第一个参数传入。

```cpp
#include "mywidget.h"

void MyWidget::doSomething() {
    // 记录一条信息(Info)级别的日志
    qCInfo(logMyWidget) << "Starting the complex operation with value:" << someValue;

    bool success = performOperation();

    if (!success) {
        // 记录一条警告(Warning)级别的日志
        qCWarning(logMyWidget) << "The operation completed with a non-critical error.";
    } else {
        qCDebug(logMyWidget) << "Operation successful. Intermediate result:" << result;
    }
}
```

### 日志级别

请根据消息的严重性选择合适的日志级别：

| 宏 | 级别 | 用途 |
| :--- | :--- | :--- |
| `qCDebug(category)` | Debug | 用于开发调试。在 Release 构建中会被完全优化掉，无性能影响。 |
| `qCInfo(category)` | Info | 用于记录应用流程中的关键信息，如“服务已启动”、“文件已加载”。 |
| `qCWarning(category)` | Warning | 用于报告可恢复的异常情况，程序可以继续运行。 |
| `qCCritical(category)` | Critical | 用于报告严重错误，可能导致部分功能失效。 |
| `qFatal("message")` | Fatal | **（注意：不带类别）** 用于报告导致程序无法继续运行的致命错误。**调用后程序会立即终止。** |

-----

## 3\. 查看与配置日志

### 查看日志文件

`QWLogger` 会将所有日志输出到 `app.log` 文件中。您可以在以下标准应用数据目录中找到它：

  * **Windows**: `C:\Users\<YourUser>\AppData\Local\<YourCompany>\QtWin\app.log`
  * **Linux**: `/home/<YourUser>/.local/share/<YourCompany>/QtWin/app.log`
  * **macOS**: `/Users/<YourUser>/Library/Application Support/<YourCompany>/QtWin/app.log`

> **注意**: `<YourCompany>` 和 `QtWin` 是在 `main.cpp` 中通过 `QWApplication` 构造函数设置的组织和应用名称。

日志条目的格式如下：
`[时间戳][级别] 消息 (源文件:行号, 函数名)`

**示例：**
`[2025-06-28 10:30:15.123][INFO] qtwin.ui.mywidget: Starting the complex operation with value: 42 (mywidget.cpp:50, void QtWin::MyWidget::doSomething())`

### 在运行时配置日志

这是本日志系统最强大的功能之一。您可以在不重新编译程序的情况下，通过设置环境变量来动态调整日志的详细程度。

**环境变量**: `QT_LOGGING_RULES`

**规则格式**: `<category>.<type>=<true|false>`，多条规则用分号 `;` 分隔。

**常用场景示例：**

  * **场景一：关闭所有调试日志，只看警告和错误。**
    ```shell
    export QT_LOGGING_RULES="*.debug=false;*.info=false"
    ```

  * **场景二：应用出现异常，需要详细查看网络模块和主窗口的调试信息。**
    ```shell
    export QT_LOGGING_RULES="*.debug=false;qtwin.core.network.debug=true;qtwin.ui.mainwindow.debug=true"
    ```
这条规则会关闭所有其他模块的 `debug` 日志，只保留 `network` 和 `mainwindow` 两个类别的 `debug` 输出，让您能专注于分析问题，避免被海量无关日志淹没。
