find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found.")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CAMKE_AUTORCC ON)

set(LIB_HEADERS
    include/qtwDWM.h
    include/qtwException.h
)

set(LIB_SOURCES
    qtwDWM.cpp
    qtwException.cpp
)

add_library(QtWin SHARED
    ${LIB_SOURCES}
    ${LIB_HEADERS}
)

# ---符号导出管理---
include(GenerateExportHeader)
generate_export_header(QtWin
    BASE_NAME QtWin
    EXPORT_MACRO_NAME QTWIN_API
    EXPORT_FILE_NAME QtWin.h
)

set_target_properties(QtWin PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS FALSE)

target_link_libraries(QtWin
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
if(WIN32)
    target_link_libraries(QtWin PRIVATE dwmapi)
endif()

target_include_directories(QtWin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# --- 安装规则 ---
install(TARGETS QtWin
    EXPORT QtWinTargets
    RUNTIME DESTINATION library COMPONENT Runtime
    LIBRARY DESTINATION library COMPONENT Runtime
    ARCHIVE DESTINATION library COMPONENT Devel
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Devel
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/QtWin_export/h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Devel
)

install(EXPORT QtWinTargets
    FILE QtWinTargets.cmake
    NAMESPACE QtWin::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtWin
    COMPONENT Devel
)


