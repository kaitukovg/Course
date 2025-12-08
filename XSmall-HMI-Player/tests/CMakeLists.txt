cmake_minimum_required(VERSION 3.15)
project(HMI_Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Configuring tests...")

set(TEST_SOURCES
    test_main.cpp
    test_variable_database.cpp
    test_visual_objects.cpp
    test_scene_factory.cpp
)

add_executable(HMI_Tests ${TEST_SOURCES})

# Директории с заголовками
target_include_directories(HMI_Tests PRIVATE 
    ../include
    ${sfml_SOURCE_DIR}/include
)

target_sources(HMI_Tests PRIVATE
    ../src/VariableDatabase.cpp
    ../src/VisualObject.cpp
    ../src/Rectangle.cpp
    ../src/Text.cpp
    ../src/Line.cpp
    ../src/Polyline.cpp
    ../src/InputField.cpp
    ../src/Button.cpp
    ../src/Image.cpp
    ../src/HistoryGraph.cpp
    ../src/SceneFactory.cpp
)

# Для статической линковки
target_compile_definitions(HMI_Tests PRIVATE SFML_STATIC)

target_link_libraries(HMI_Tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    sfml-graphics
    sfml-window
    sfml-system
)

if(WIN32)
    target_link_libraries(HMI_Tests PRIVATE
        opengl32
        winmm
        gdi32
    )
endif()

message(STATUS "Tests configured")
