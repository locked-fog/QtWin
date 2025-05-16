cmake_minimum_required(VERSION 3.16)

project(QtWin VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found. Please install Qt6.")
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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

target_link_libraries(QtWin PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(WIN32)
    target_link_libraries(QtWin PRIVATE dwmapi)
endif()

target_include_directories(QtWin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

