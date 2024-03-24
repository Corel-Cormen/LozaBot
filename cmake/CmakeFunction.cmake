function(copy_files_to_target target files path)
    get_target_property(target_bin_path ${target} RUNTIME_OUTPUT_DIRECTORY)

    foreach(FILE ${files})

        if(NOT EXISTS ${target_bin_path}/${FILE})
            add_custom_command(TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${path}/${FILE} ${target_bin_path}
            )
        endif()

    endforeach()
endfunction()
