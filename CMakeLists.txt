cmake_minimum_required(VERSION 3.16)

project(QtWin VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

add_subdirectory(src)

# --- CMake配置 ---
include(CMakePackageConfigHelpers)

# QtWinConfig.cmake.in 模板文件应位于 ${CMAKE_SOURCE_DIR}/cmake/ 目录下
set(ConfigPackageLocation "library/cmake/QtWin")

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/QtWinConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfig.cmake"
    INSTALL_DESTINATION ${ConfigPackageLocation}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_BINDIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfigVersion.cmake"
    DESTINATION ${ConfigPackageLocation}
    COMPONENT Devel
)


