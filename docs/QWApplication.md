
### `QWApplication` 用户开发指南

这份指南面向希望在项目中使用 `QWApplication` 类的开发者。

#### 集成到项目中

要使用 `QWApplication`，您需要修改项目的主入口文件（通常是 `main.cpp`）。用 `QWApplication` 替换标准的 `QApplication` 实例化。

**`main.cpp` 示例：**

```cpp
#include "QWApplication.h"
#include "YourMainWindow.h" // 假设您的主窗口类名为 YourMainWindow

int main(int argc, char *argv)
{
    // 1. 实例化 QWApplication，并提供应用名称和版本。
    QWApplication app(argc, argv, "QtWin", "1.0.0");

    // 2. 创建您的主窗口。
    YourMainWindow window;
    window.show();

    // 3. 启动事件循环。
    return app.exec();
}
```

#### 全局访问与使用

在应用的任何地方，您都可以通过静态的 `instance()` 方法安全地访问 `QWApplication` 的实例和它的自定义功能。

**示例：在某个控件中获取应用名称和主题模式**

```cpp
#include "QWApplication.h"
#include <QDebug>

void MyCustomWidget::someMethod()
{
    // 获取应用名称
    qDebug() << "Application Name:" << QWApplication::instance()->applicationName();

    // 检查当前主题模式
    if (QWApplication::instance()->isDarkMode()) {
        qDebug() << "Currently in Dark Mode.";
    } else {
        qDebug() << "Currently in Light Mode.";
    }
}
```

#### 示例：响应深色模式切换

您可以将任何控件的槽函数连接到 `darkModeChanged` 信号，以实现对主题变化的实时响应。

**示例：一个能自动切换背景色的 `QLabel`**

```cpp
// MyThemedLabel.h
#include <QLabel>

class MyThemedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyThemedLabel(const QString &text, QWidget *parent = nullptr);

private slots:
    void onThemeChanged(bool isDark);
};

// MyThemedLabel.cpp
#include "MyThemedLabel.h"
#include "QWApplication.h"

MyThemedLabel::MyThemedLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    // 连接到全局的 darkModeChanged 信号
    connect(QWApplication::instance(), &QWApplication::darkModeChanged,
            this, &MyThemedLabel::onThemeChanged);

    // 初始化当前主题
    onThemeChanged(QWApplication::instance()->isDarkMode());
}

void MyThemedLabel::onThemeChanged(bool isDark)
{
    QPalette p = palette();
    if (isDark) {
        p.setColor(QPalette::Window, Qt::darkGray);
        p.setColor(QPalette::WindowText, Qt::white);
    } else {
        p.setColor(QPalette::Window, Qt::lightGray);
        p.setColor(QPalette::WindowText, Qt::black);
    }
    setAutoFillBackground(true);
    setPalette(p);
}
```