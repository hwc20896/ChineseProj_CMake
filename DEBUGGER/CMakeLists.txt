cmake_minimum_required(VERSION 3.19)
project(DEBUGGER LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(DEBUGGER
    WIN32 MACOSX_BUNDLE
    main.cpp
    src/cpp/widget.cpp
    src/headers/widget.h
)

target_link_libraries(DEBUGGER
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

include(GNUInstallDirs)

install(TARGETS DEBUGGER
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

target_include_directories(DEBUGGER
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/headers
)

qt_generate_deploy_app_script(
    TARGET DEBUGGER
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
