cmake_minimum_required(VERSION 3.18)
project(DocsGenerator)

set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(ENV{QT_AUTO_SCREEN_SCALE_FACTOR} 1)

set(REQUIRED_LIBS Core Gui Widgets WebKitWidgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::WebKitWidgets)

add_executable(${PROJECT_NAME} main.cpp Form.cpp Form.h PreviewWorker.cpp PreviewWorker.h icons.qrc)


find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
