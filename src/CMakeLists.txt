qt_standard_project_setup()

qt_add_library(QtWin SHARED
    qwpalette.cpp
    qwapplication.cpp
    qwlogger.cpp
    qwsettings.cpp
    qwwindow.cpp
    qwtabbutton.cpp
    qwnavigationview.cpp
    ../include/QtWin/QWPalette.h
    ../include/QtWin/QWApplication.h
    ../include/QtWin/QWLogger.h
    ../include/QtWin/QWSettings.h
    ../include/QtWin/QWWindow.h
    ../include/QtWin/QWTabButton.h
    ../include/QtWin/QWNavigationView.h
)

add_library(QtWin::QtWin ALIAS QtWin)

target_include_directories(QtWin
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
        "$<INSTALL_INTERFACE:include>"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(QtWin
    PUBLIC
        Qt6::Widgets
    PRIVATE
        $<$<PLATFORM_ID:Windows>:dwmapi>
)

set_target_properties(QtWin PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}

    WINDOWS_EXPORT_ALL_SYMBOLS ON

    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED}
)

# =======================
# Installation Rules
# =======================

include(GNUInstallDirs)

install(TARGETS QtWin
    EXPORT QtWinTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Development
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/QtWinConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtWin
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QtWinConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtWin
    COMPONENT Development
)

install(EXPORT QtWinTargets
    FILE QtWinTargets.cmake
    NAMESPACE QtWin::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtWin
    COMPONENT Development
)