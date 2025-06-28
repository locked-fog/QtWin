# QtWin 配置管理系统 (QWSettings) 开发手册

## 1. 概述

`QWSettings` 是 `QtWin` 控件库提供的统一配置管理系统。它封装了 Qt 的 `QSettings` 类，提供全局统一的配置管理服务。

**核心设计理念：**

* **统一访问点**：作为应用唯一的配置访问点，通过 `QWApplication::instance()->settings()` 全局访问
* **自动路径管理**：自动处理配置文件路径解析（默认路径或自定义路径）
* **线程安全**：内部使用智能指针管理 `QSettings` 实例，确保生命周期安全
* **INI格式**：强制使用 INI 文件格式存储配置，保证可读性和兼容性

默认配置文件路径遵循各平台标准：
* **Windows**: `C:\Users\<User>\AppData\Local\<OrgName>\<AppName>\settings.conf`
* **Linux**: `/home/<User>/.config/<OrgName>/<AppName>/settings.conf` 
* **macOS**: `/Users/<User>/Library/Application Support/<OrgName>/<AppName>/settings.conf`

-----

## 2. 如何使用

### 2.1 获取配置实例

配置系统由 `QWApplication` 自动初始化，可通过以下方式获取：

```cpp
// 获取全局配置实例
QtWin::QWSettings* settings = QtWin::QWApplication::instance()->settings();
```

### 2.2 基本读写操作

#### 写入配置
```cpp
settings->setValue("window/size", QSize(800, 600));
settings->setValue("recentFiles", QStringList{"file1.txt", "file2.txt"});
```

#### 读取配置
```cpp
QSize size = settings->value("window/size", QSize(400, 300)).toSize();
QStringList files = settings->value("recentFiles").toStringList();
```

#### 检查配置项
```cpp
if (settings->contains("window/maximized")) {
    // 配置项存在
}
```

#### 删除配置项
```cpp
settings->remove("obsoleteSetting");
```

### 2.3 分组管理

使用分组可以组织相关配置项：

```cpp
settings->beginGroup("Network");
settings->setValue("timeout", 30);
settings->setValue("retryCount", 3);
settings->endGroup();

// 读取时也需要使用相同分组
settings->beginGroup("Network");
int timeout = settings->value("timeout").toInt();
settings->endGroup();
```

### 2.4 强制同步

配置变更通常会自动保存，但可以强制立即同步：

```cpp
settings->sync();  // 立即写入磁盘并重新加载
```

-----

## 3. 高级功能

### 3.1 自定义配置文件路径

可以在构造时或运行时指定自定义路径：

```cpp
// 构造时指定
QWSettings* settings = new QWSettings(parent, "/path/to/custom.conf");

// 运行时修改
settings->setFilePath("/new/path/settings.conf");
```

> **注意**：传入空路径将恢复为默认路径

### 3.2 获取当前配置文件路径

```cpp
QString path = settings->filePath();
qDebug() << "Current config file:" << path;
```

-----

## 4. 最佳实践

1. **键命名规范**：
   * 使用小写字母和`/`作为分隔符（如`ui/mainwindow/size`）
   * 保持命名一致性和可读性

2. **类型安全**：
   * 读取时总是指定默认值和转换类型
   ```cpp
   int value = settings->value("timeout", 30).toInt();
   ```

3. **性能考虑**：
   * 避免频繁调用`sync()`，Qt会自动在适当时候保存
   * 批量操作时使用分组减少重复前缀

4. **线程安全**：
   * 所有配置操作应在主线程执行
   * 如需跨线程访问，通过信号槽机制传递配置值
