cmake_minimum_required(VERSION 3.16)
project(OrnamentServer2.0 LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE  HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Network
        Sql
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    cpp.hint
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

qt_add_executable(OrnamentServer2.0 ${PROJECT_SOURCES})

target_link_libraries(OrnamentServer2.0
    PUBLIC
        Qt6::Core
        Qt6::Network
        Qt6::Sql
)
