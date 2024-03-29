set(TARGET_LIST "" CACHE INTERNAL "executable targets lists in system")

macro(register_test test_target_name)
    list(APPEND TARGET_LIST ${test_target_name})
    set(TARGET_LIST ${TARGET_LIST} CACHE STRING "" FORCE)
    set_target_properties(${test_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/UnitTests/bin")
    copy_files_to_target(${test_target_name} "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})
    add_test(NAME ${test_target_name} COMMAND ${test_target_name} --gtest_shuffle)
endmacro()

include_directories(_mock)

set(GTEST_LIBS
    gtest
    gmock
    gtest_main
    gmock_main
)

add_subdirectory(RequestDriverTest)
