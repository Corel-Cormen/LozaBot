macro(get_source_properties lib_name)
    set(LIB_API ${CMAKE_CURRENT_SOURCE_DIR}/../../App/${lib_name}/api)
    set(LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../App/${lib_name}/src)
    get_target_property(LIB_DEPENDS ${lib_name} LINK_LIBRARIES)
    file(GLOB_RECURSE LIB_SOURCE
        "${LIB_SRC}/*.cpp"
        "${LIB_SRC}/*.hpp"
        "${LIB_API}/*.hpp"
    )
endmacro()

macro(register_test test_target_name)
    set_target_properties(${test_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/UnitTests/bin")
    copy_files_to_target(${test_target_name} "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})
    add_test(NAME ${test_target_name} COMMAND ${test_target_name} --gtest_shuffle)
endmacro()

include_directories(_mock)

add_subdirectory(3rdparty)
set(GTEST_LIBS
    gtest
    gmock
    gtest_main
    gmock_main
)

add_subdirectory(RequestDriverTest)
add_subdirectory(RequestControllerTest)
add_subdirectory(RequestDataCacheTest)
