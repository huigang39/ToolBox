cmake_minimum_required(VERSION 3.22)
project(ToolBox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        PrintSupport
        REQUIRED
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/plot)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/signal)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/qcustomplot)

file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/plot/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/signal/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/qcustomplot/*.cpp
)

file(GLOB_RECURSE HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/plot/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/signal/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/qcustomplot/*.h
)

file(GLOB_RECURSE UI
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui
)

file(GLOB_RECURSE RES
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.qrc
)

set(PROJECT_SOURCES
        ${SOURCES}
        ${HEADERS}
        ${UI}
        ${RES}
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::PrintSupport
)

# if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#     set(DEBUG_SUFFIX)
#     if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
#         set(DEBUG_SUFFIX "d")
#     endif ()
#     set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
#     if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#         set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#         if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#             set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#         endif ()
#     endif ()
#     if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                 COMMAND ${CMAKE_COMMAND} -E make_directory
#                 "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                 COMMAND ${CMAKE_COMMAND} -E copy
#                 "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
#                 "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#     endif ()
#     foreach (QT_LIB Core Gui Widgets)
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                 COMMAND ${CMAKE_COMMAND} -E copy
#                 "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
#                 "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
#     endforeach (QT_LIB)
# endif ()
