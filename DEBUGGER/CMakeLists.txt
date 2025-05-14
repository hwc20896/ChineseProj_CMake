cmake_minimum_required(VERSION 3.19)
project(DEBUGGER LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

set(CMAKE_CXX_STANDARD 23)

qt_standard_project_setup()

qt_add_executable(DEBUGGER
    WIN32 MACOSX_BUNDLE
    main.cpp

    #  cpp
    src/cpp/widget.cpp
    src/cpp/questioncontainer.cpp
    src/cpp/viewer.cpp
    src/cpp/property.cpp

    #  headers
    src/headers/widget.h
    src/headers/questioncontainer.h
    src/headers/viewer.h
    src/headers/property.h

    #  UIs
    src/ui/main.ui
    src/ui/questioncontainer.ui
    src/ui/questionview.ui
    src/ui/property.ui

    #  QRCs
    resources.qrc
)

set_target_properties(DEBUGGER PROPERTIES
    AUTOUIC ON
    AUTORCC ON
    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
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
        ${CMAKE_CURRENT_BINARY_DIR}
)

qt_generate_deploy_app_script(
    TARGET DEBUGGER
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
