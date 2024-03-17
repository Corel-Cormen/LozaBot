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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_subdirectory(3rdparty)

add_subdirectory(App/RequestDriver)

# --- Application target properties ---

add_executable(LozaBot
    main.cpp
)

target_link_libraries(LozaBot
    Qt${QT_VERSION_MAJOR}::Core
    fmt
    RequestDriver
)

# --- Unit Test target properties ---

add_executable(LozaBotTest
    tst_testcase.cpp
)

target_link_libraries(LozaBotTest
    PUBLIC
    gtest
    gmock
    gtest_main
    gmock_main
    fmt
)

add_test(NAME LozaBotUnitTest COMMAND LozaBotTest --gtest_shuffle)

get_property(targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${targets})
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        set_target_properties(${target}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/bin"
        )
    endif()
endforeach()

include(cmake/QtCompileProperties.cmake)
