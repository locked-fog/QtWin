# QtWin

一个现代化的Qt6/C++部件库，采用类似WinUI3的美学设计，用于构建美观简洁的桌面应用程序。

## 功能特性

- TODO

## 截图展示

- TODO

## 从源码构建

按照以下步骤从源代码构建QtWin库。

### 前提条件

- 支持C++17的编译器(如MSVC 2019+, GCC 9+, Clang 9+)
- CMake(3.16或更新版本)
- Qt(6.2或更新版本)

### 构建步骤

1. 克隆仓库:

```bash
git clone https://github.com/locked-fog/QtWin.git
cd QtWin
```

2. 创建构建目录并使用CMake配置项目。这是标准的out-of-source构建方式。

```bash
mkdir build
cd build
```

3. 运行CMake生成构建文件。

  - 如果Qt已在系统PATH或标准安装位置:

  ```bash
  cmake..
  ```

  - 如果Qt安装在自定义位置，需要通过设置`CMAKE_PREFIX_PATH`告诉CMake其位置:

  ```bash
  # Windows系统
  cmake.. -DCMAKE_PREFIX_PATH="C:/Qt/6.5.3/msvc2019_64"
  
  # Linux/macOS系统
  cmake.. -DCMAKE_PREFIX_PATH="/path/to/your/Qt/6.5.3/gcc_64"
  ```

4. 编译库:

```bash
cmake --build. --config Release
```

5. (可选)使用CPack创建可分发包。这将在build目录中生成.zip(Windows)或.tar.gz(Linux/macOS)归档文件。

```bash
cpack -G ZIP  # 或 -G TGZ
```

## 在项目中使用QtWin

构建并打包QtWin后，可以轻松将其集成到基于CMake的应用程序中。

1. **安装QtWin**: 将CPack生成的归档文件(如`QtWin-1.0.0-Windows.zip`)解压到系统的稳定位置。此位置将是您的`<qtwin_install_dir>`。

2. **配置项目的`CMakeLists.txt`**: 在应用程序的`CMakeLists.txt`中，使用`find_package`定位QtWin，并使用`target_link_libraries`链接它。

```CMake
cmake_minimum_required(VERSION 3.16)
project(MyAwesomeApp)

# 查找依赖项
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(QtWin 1.0.0 REQUIRED) # 查找QtWin库

# 创建应用程序目标
qt_standard_project_setup()
qt_add_executable(MyAwesomeApp main.cpp)

# 链接QtWin
# 这会自动处理包含目录和其他依赖项
target_link_libraries(MyAwesomeApp PRIVATE QtWin::QtWin)
```

3. **构建应用程序**: 使用CMake配置应用程序时，必须将`CMAKE_PREFIX_PATH`指向安装QtWin的目录。这允许`find_package`定位必要的配置文件(`QtWinConfig.cmake`)。

```bash
# 为应用程序创建构建目录
mkdir build
cd build

# 配置，必要时指向Qt和QtWin的安装位置
# (Windows上用分号分隔路径，Linux/macOS上用冒号)
cmake.. -DCMAKE_PREFIX_PATH="<path_to_qt_install>;<path_to_qtwin_install>"

# 构建应用程序
cmake --build.
```

## 贡献指南

TODO

## 许可证

> 本项目使用Apache 2.0许可证。
