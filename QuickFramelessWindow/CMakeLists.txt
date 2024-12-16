cmake_minimum_required(VERSION 3.30)
project(FramelessWindow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "D:/AppData/QT/6.8.1/msvc2022_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(LIB "dwmapi.lib")
find_package(Qt6 6.8 REQUIRED COMPONENTS Quick)

qt_add_executable(${PROJECT_NAME} main.cpp res.qrc
        QuickFramelessWindow.cpp
        QuickFramelessWindow.h
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick ${LIB})
