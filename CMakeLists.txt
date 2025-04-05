cmake_minimum_required(VERSION 3.19)
project(ChineseProj_CMake LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

set(CMAKE_CXX_STANDARD 23)

qt_standard_project_setup()

qt_add_executable(ChineseProj_CMake
    WIN32 MACOSX_BUNDLE

    main.cpp

    #  cpps
    src/cpp/mainwidget.cpp
    src/cpp/introwidget.cpp
    src/cpp/rulewidget.cpp
    src/cpp/template.cpp
    src/cpp/managementwidget.cpp

    #  headers
    src/header/mainwidget.h
    src/header/introwidget.h
    src/header/rulewidget.h
    src/header/template.h
    src/header/managementwidget.h

    #  UIs
    src/ui/introwidget.ui
    src/ui/rulewidget.ui
    src/ui/managementwidget.ui
    src/header/questionwidget.h

    #  QRCs
    resources.qrc

    #  CSSs
    src/css/regular.css
)

set_target_properties(ChineseProj_CMake PROPERTIES
    AUTOUIC ON
    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

target_link_libraries(ChineseProj_CMake
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Multimedia
)

target_include_directories(ChineseProj_CMake
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/header
)

include(GNUInstallDirs)

install(TARGETS ChineseProj_CMake
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ChineseProj_CMake
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
