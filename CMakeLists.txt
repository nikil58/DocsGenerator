cmake_minimum_required(VERSION 3.18)
project(DocsGenerator)

set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 6)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(REQUIRED_LIBS Core Gui Widgets WebEngineWidgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets  Qt6::WebEngineWidgets)

add_executable(${PROJECT_NAME} main.cpp Form.cpp Form.h PreviewWorker.cpp PreviewWorker.h icons.qrc)


find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
