# QtWin/tests/CMakeLists.txt

# 1. 定义测试项目的名称。
project(QtWinTests)

# 2. 使用 Qt 提供的辅助函数，自动处理 moc, uic, rcc。
#    这对于任何使用 Qt 的目标都是一个好习惯。
qt_standard_project_setup()

# 3. 创建一个可执行文件目标。
#    目标名称为 "QtWinTestApp"，源文件是 "main.cpp"。
qt_add_executable(QtWinTestApp
    main.cpp
)

# 4. 链接依赖库。这是最关键的一步。
target_link_libraries(QtWinTestApp
    PRIVATE
        # 链接到我们自己开发的 QtWin 库。
        # CMake 会自动处理头文件路径和库文件的链接。
        # 使用 QtWin::QtWin 别名是现代 CMake 的最佳实践。
        QtWin::QtWin

        # 我们的测试程序本身也需要 Qt 的 Widgets 模块
        # 来创建 QApplication 和 QMainWindow。
        Qt6::Widgets
)

if(WIN32)
    add_custom_command(
        TARGET QtWinTestApp       # 指定这个命令附加到哪个目标上
        POST_BUILD                # 指定在目标构建成功后执行
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:QtWin>  # 要复制的文件 (QtWin.dll)
            $<TARGET_FILE_DIR:QtWinTestApp> # 目标目录 (exe所在的目录)
        COMMENT "Copying QtWin.dll to test executable directory..."
    )
endif()