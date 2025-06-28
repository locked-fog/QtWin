# QtWin开发手册与简单计划书

1. 本项目为一个基于C++和Qt6的控件库，以及与之配套的一个完整的开发和设计方案

2. 本项目通过CMake进行项目管理，已托管至 `https://github.com/locked-fog/QtWin`

3. 本项目的灵感来源是 Fluent Design ，颜色设计部分参考Material You 中的 Dynamic Color ，设计语言参考 Ant Design ，目标是打造一套便于开发、维护和使用的组件系统和开发思路，提供C++/Qt6的更优秀解决方案

4. 项目开发理念：软件承载窗口（Window），窗口承载页面（Tab，但是实际表现应当接近WinUI3中的NavigationView），页面承载布局（Layout），布局承载控件（Widget），控件接受操作并给出反馈。一个窗口的设计以层叠的思路为主，通过颜色变化展示遮挡关系。操作直观符合直觉。

5. 项目开发周期：在AI工具的协助下，预期开发周期不超过2个月发布第一个测试版本

6. 文件管理规范：头文件放置于`/include/QtWin/`，统一命名为 `QWXxxxxxx.h` ，例如实现日志类 `QWLogger` 的头文件应当命名为 `QWLogger.h`。源文件放置于`/src/`，命名为对应头文件的全小写，例如上述文件的源文件应当命名为 `qwlogger.cpp`，开发手册放置于`/docs/`下，命名为对应头文件名，例如上述文件的开发手册应当命名为 `QWLogger.md`

7. 代码规范：
  1. 头文件：使用下面的结构避免重复引用，并提高代码可读性
  ```cpp
  #ifndef XXX_H
  #define XXX_H

  //other content

  #endif
  ```
  2. 命名规范：参考其他C++项目，命名规范不赘述

  3. 代码细节：参考`类Java`语法，大括号不单独一行，合并至上一行，例如：

  ```cpp
  int main(){ //大括号在这一行

  }
  ```