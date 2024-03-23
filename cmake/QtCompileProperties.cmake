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
set(SSH_LIB_PATH "" CACHE STRING "Ssh lib tools path")

if(NOT CXX_COMPILER_LIB_PATH AND
        NOT QT_COMPILER_LIB_PATH AND
        NOT SSH_LIB_PATH)
    message(STATUS "Configuration based on the path to the Qt compiler")

    get_filename_component(CXX_COMPILER_LIB_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)

    string(FIND ${QT_DIR} "/lib/" LIB_PATH_INDEX REVERSE)
    string(SUBSTRING ${QT_DIR} 0 ${LIB_PATH_INDEX} QT_COMPILER_LIB_PATH)
    set(QT_COMPILER_LIB_PATH ${QT_COMPILER_LIB_PATH}/bin)

    set(SSH_PREFIX_PATH "/Tools/")
    string(LENGTH SSH_PREFIX_PATH SSH_PREFIX_PATH_LEN)
    string(FIND ${CMAKE_CXX_COMPILER} ${SSH_PREFIX_PATH} LIB_PATH_INDEX REVERSE)
    math(EXPR SSH_INDEX "${LIB_PATH_INDEX}+((${SSH_PREFIX_PATH_LEN}-1)/2)")
    string(SUBSTRING ${CMAKE_CXX_COMPILER} 0 ${SSH_INDEX} SSH_LIB_PATH)

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(SSH_LIB_PATH ${SSH_LIB_PATH}OpenSSL/Win_x64/bin)
        set(SHH_LIB_LIST_FILES
            libcrypto-1_1-x64.dll
            libssl-1_1-x64.dll
        )
    elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        set(SSH_LIB_PATH ${SSH_LIB_PATH}OpenSSL/Win_x86/bin)
        set(SHH_LIB_LIST_FILES
            libcrypto-1_1-x86.dll
            libssl-1_1-x86.dll
        )
    else()
        message(FATAL_ERROR "Not supported system architecture")
    endif()

else()
    message(STATUS "Paths defined by user")
endif()

message(STATUS "CXX libs path: ${CXX_COMPILER_LIB_PATH}")
message(STATUS "Qt libs path: ${QT_COMPILER_LIB_PATH}")
message(STATUS "Ssh libs path: ${SSH_LIB_PATH}")

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
copy_files_to_target(LozaBot "${SHH_LIB_LIST_FILES}" ${SSH_LIB_PATH})
copy_files_to_target(LozaBotTest "${CXX_LIB_LIST_FILES_APP}" ${CXX_COMPILER_LIB_PATH})

