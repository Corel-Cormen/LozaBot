add_subdirectory(3rdparty)
add_subdirectory(QtWrappers)
add_subdirectory(RequestDriver)

add_executable(LozaBot
    main.cpp
)

target_link_libraries(LozaBot
    Qt${QT_VERSION_MAJOR}::Core
    fmt
    RequestDriver
)

set_target_properties(LozaBot PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LozaBot/bin")

copy_files_to_target(LozaBot "${QT_LIB_LIST_FILES_APP}" ${QT_COMPILER_LIB_PATH})
copy_files_to_target(LozaBot "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})
copy_files_to_target(LozaBot "${SHH_LIB_LIST_FILES}" ${SSH_LIB_PATH})
