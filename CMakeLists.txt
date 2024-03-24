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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CXX_COMPILER_LIB_PATH "" CACHE STRING "C++ libs compile tools bin path")
set(QT_COMPILER_LIB_PATH "" CACHE STRING "Qt libs compile tools bin path")
set(SSH_LIB_PATH "" CACHE STRING "Ssh lib tools path")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include(cmake/QtWinFilesDefined.cmake)
include(cmake/CmakeFunction.cmake)

add_subdirectory(3rdparty)

include_directories(App/Common)
add_subdirectory(App)
add_subdirectory(UnitTests)
