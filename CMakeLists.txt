cmake_minimum_required(VERSION 3.18)
project(DocsGenerator VERSION 1.4.5)

set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(REQUIRED_LIBS Core Gui Widgets WebEngineWidgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::WebEngineWidgets)

set(SOURCES_FILES main.cpp Form.cpp PreviewWorker.cpp Spoiler.cpp CharacterForm.cpp ToggleButtonR.cpp)
set(HEADERS_FILES Form.h PreviewWorker.h Spoiler.h CharacterForm.h ToggleButtonR.h)

add_compile_definitions(ALL_PROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\")
add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEADERS_FILES} icons.qrc)


find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
