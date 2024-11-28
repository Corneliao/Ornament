cmake_minimum_required(VERSION 3.16)
project(Ornament LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(app_icon_resource_windows "${PROJECT_SOURCE_DIR}/Resource/ico.rc")

FILE(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
FILE(GLOB_RECURSE LOGIN_HEADER_FILES ${PROJECT_SOURCE_DIR}/Login/include/*.h)
FILE(GLOB_RECURSE LOGIN_COMPONENT_HEADER_FILES ${PROJECT_SOURCE_DIR}/Login/Component/include/*.h)
FILE(GLOB_RECURSE COMPONENT_HEADER_FILES ${PROJECT_SOURCE_DIR}/Component/include/*.h)
FILE(GLOB_RECURSE DELEGATE_HEADER_FILES ${PROJECT_SOURCE_DIR}/StyledItemDelegate/include/*.h)
FILE(GLOB_RECURSE CHATPAGE_HEADER_FILES ${PROJECT_SOURCE_DIR}/ChatPage/include/*.h)
FILE(GLOB_RECURSE FRIENDPAGE_HEADER_FILES ${PROJECT_SOURCE_DIR}/FriendPage/include/*.h)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/Login/src LOGIN_SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/Login/Component/src LOGIN_COMPONENT_SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/Component/src COMPONENT_SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/StyledItemDelegate/src  DELEGATE_SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ChatPage/src CHATPAGE_SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/FriendPage/src FRIENDPAGE_SOURCE_FILES)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    cpp.hint
    Resource.qrc
    FramelessWindow/include/framelesswindow.h
    FramelessWindow/src/framelesswindow.cpp
    UserDataManager/include/UserDatabaseManager.h
    UserDataManager/src/UserDatabaseManager.cpp
    Login/Register/include/RegisterUserAccount.h
    Login/Register/src/RegisterUserAccount.cpp
    global.h
    global.cpp
    ${app_icon_resource_windows}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${LOGIN_HEADER_FILES}
    ${LOGIN_SOURCE_FILES}
    ${LOGIN_COMPONENT_HEADER_FILES}
    ${LOGIN_COMPONENT_SOURCE_FILES}
    ${COMPONENT_HEADER_FILES}
    ${COMPONENT_SOURCE_FILES}
    ${DELEGATE_HEADER_FILES}
    ${DELEGATE_SOURCE_FILES}
    ${CHATPAGE_HEADER_FILES}
    ${CHATPAGE_SOURCE_FILES}
    ${FRIENDPAGE_HEADER_FILES}
    ${FRIENDPAGE_SOURCE_FILES}
)

qt_add_executable(Ornament ${PROJECT_SOURCES})

set_target_properties(Ornament
    PROPERTIES
        WIN32_EXECUTABLE TRUE
        OUTPUT_NAME "Ornament"
)

target_link_libraries(Ornament
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
        Qt6::Network
)
