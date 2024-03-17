macro(copy_files_to_target target files path)
    get_target_property(target_bin_path ${target} RUNTIME_OUTPUT_DIRECTORY)

    foreach(FILE ${files})

        if(NOT EXISTS ${target_bin_path}/${FILE})
            add_custom_command(TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${path}/${FILE} $<TARGET_FILE_DIR:${target}>
            )
        endif()

    endforeach()
endmacro()

set(CXX_COMPILER_LIB_PATH "" CACHE STRING "C++ libs compile tools bin path")
set(QT_COMPILER_LIB_PATH "" CACHE STRING "Qt libs compile tools bin path")

if(NOT CXX_COMPILER_LIB_PATH AND NOT QT_COMPILER_LIB_PATH)
    message(STATUS "Configuration based on the path to the Qt compiler")

    get_filename_component(CXX_COMPILER_LIB_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)
    string(FIND ${QT_DIR} "/lib" LIB_PATH_INDEX REVERSE)
    string(SUBSTRING ${QT_DIR} 0 ${LIB_PATH_INDEX} QT_COMPILER_LIB_PATH)
    set(QT_COMPILER_LIB_PATH ${QT_COMPILER_LIB_PATH}/bin)
else()
    message(STATUS "Paths defined by user")
endif()

message(STATUS "CXX libs path: ${CXX_COMPILER_LIB_PATH}")
message(STATUS "Qt libs path: ${QT_COMPILER_LIB_PATH}")

set(QT_LIB_LIST_FILES_APP
    Qt5Core.dll
)
set(CXX_LIB_LIST_FILES_APP
    libwinpthread-1.dll
    libstdc++-6.dll
    libgcc_s_seh-1.dll
)

copy_files_to_target(LozaBot "${QT_LIB_LIST_FILES_APP}" ${QT_COMPILER_LIB_PATH})
copy_files_to_target(LozaBot "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})
copy_files_to_target(LozaBotTest "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})

