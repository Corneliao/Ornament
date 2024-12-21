cmake_minimum_required(VERSION 3.16)
project(Ornament)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "D:/AppData/QT/6.8.1/msvc2022_64")

set(window_ico "${PROJECT_SOURCE_DIR}/resource/window_ico.rc")

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(dwmapi_lib "dwmapi.lib")

FILE(GLOB_RECURSE HEADER_FILE ${PROJECT_SOURCE_DIR}/include/*.h)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILE)
find_package(Qt6 REQUIRED COMPONENTS Quick ShaderTools)

qt_add_executable(${PROJECT_NAME} WIN32
        ${window_ico}
        QuickFramelessWindow/QuickFramelessWindow.h
        QuickFramelessWindow/QuickFramelessWindow.cpp
        main.cpp
        res.qrc
        ${HEADER_FILE}
        ${SOURCE_FILE}
)


target_link_libraries(Ornament PRIVATE Qt6::Quick ${dwmapi_lib})

include(GNUInstallDirs)
install(TARGETS Ornament
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
