cmake_minimum_required(VERSION 3.16)

project(QtWin VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_subdirectory(src)

option(QTWIN_BUILD_EXAMPLES "Build the QtWin examples" ON)
option(QTWIN_BUILD_TESTS "Build the QtWin tests" ON)

#if(QTWIN_BUILD_EXAMPLES)
#    add_subdirectory(example)
#endif()

#if(QTWIN_BUILD_TESTS)
#    add_subdirectory(tests)
#endif()

# ===============
# CPack Rules
# ===============

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A modern Qt6 widget library with WinUI3-like aesthetic.")
set(CPACK_PACKAGE_VENDOR "Locked_Fog")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "aboxyxy@outlook.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)