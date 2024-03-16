cmake_minimum_required(VERSION 3.14)

project(LozaBot LANGUAGES CXX)
enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

get_filename_component(COMPILER_LIB_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)
message(STATUS "Directory path containing compiler libraries: ${COMPILER_LIB_PATH}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# --- Application target properties ---

add_executable(LozaBot
    main.cpp
)

target_link_libraries(LozaBot
    Qt${QT_VERSION_MAJOR}::Core
)

target_link_directories(LozaBot
    PRIVATE
    ${COMPILER_LIB_PATH}
)

# --- Unit Test target properties ---

add_executable(LozaBotTest
    tst_testcase.cpp
)

add_subdirectory(3rdparty/googletest)

target_link_libraries(LozaBotTest
    PUBLIC
    gtest
    gmock
    gtest_main
    gmock_main
)

target_link_directories(LozaBotTest
    PRIVATE
    ${COMPILER_LIB_PATH}
)

add_test(NAME LozaBotUnitTest COMMAND LozaBotTest)
