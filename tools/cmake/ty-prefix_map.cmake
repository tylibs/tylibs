# Utilities for remapping path prefixes
#
# __generate_prefix_map Prepares the list of compiler flags for remapping
# various paths to fixed names. This is used when reproducible builds are
# required. This function also creates a gdbinit file for the debugger to remap
# the substituted paths back to the real paths in the filesystem.
function(__generate_prefix_map compile_options_var)
  set(compile_options)
  set(gdbinit_dir ${BUILD_DIR}/gdbinit)
  set(gdbinit_path "${gdbinit_dir}/prefix_map")
  tylibs_build_get_property(tylibs_path TYLIBS_PATH)
  tylibs_build_get_property(build_components BUILD_COMPONENTS)

  if(CONFIG_COMPILER_HIDE_PATHS_MACROS)
    list(APPEND compile_options "-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.")
    list(APPEND compile_options "-fmacro-prefix-map=${tylibs_path}=/TYLIBS")
  endif()

  if(CONFIG_APP_REPRODUCIBLE_BUILD)
    list(APPEND compile_options "-fdebug-prefix-map=${tylibs_path}=/TYLIBS")
    list(APPEND compile_options
         "-fdebug-prefix-map=${PROJECT_DIR}=/TYLIBS_PROJECT")
    list(APPEND compile_options "-fdebug-prefix-map=${BUILD_DIR}=/TYLIBS_BUILD")

    # Generate mapping for component paths
    set(gdbinit_file_lines)
    foreach(component_name ${build_components})
      tylibs_component_get_property(component_dir ${component_name}
                                    COMPONENT_DIR)

      string(TOUPPER ${component_name} component_name_uppercase)
      set(substituted_path "/COMPONENT_${component_name_uppercase}_DIR")
      list(APPEND compile_options
           "-fdebug-prefix-map=${component_dir}=${substituted_path}")
      string(APPEND gdbinit_file_lines
             "set substitute-path ${substituted_path} ${component_dir}\n")
    endforeach()

    # Mapping for toolchain path
    execute_process(COMMAND ${CMAKE_C_COMPILER} -print-sysroot
                    OUTPUT_VARIABLE compiler_sysroot)
    if(compiler_sysroot STREQUAL "")
      message(FATAL_ERROR "Failed to determine toolchain sysroot")
    endif()
    string(STRIP "${compiler_sysroot}" compiler_sysroot)
    get_filename_component(compiler_sysroot "${compiler_sysroot}/.." REALPATH)
    list(APPEND compile_options
         "-fdebug-prefix-map=${compiler_sysroot}=/TOOLCHAIN")
    string(APPEND gdbinit_file_lines
           "set substitute-path /TOOLCHAIN ${compiler_sysroot}\n")

    file(WRITE "${BUILD_DIR}/prefix_map_gdbinit" "${gdbinit_file_lines}"
    )# TODO TYLIBS-11667
    tylibs_build_set_property(DEBUG_PREFIX_MAP_GDBINIT "${gdbinit_path}")
  else()
    set(gdbinit_file_lines
        "# There is no prefix map defined for the project.\n")
  endif()
  # Write prefix_map_gdbinit file even it is empty.
  file(MAKE_DIRECTORY ${gdbinit_dir})
  file(WRITE "${gdbinit_path}" "${gdbinit_file_lines}")
  tylibs_build_set_property(GDBINIT_FILES_PREFIX_MAP "${gdbinit_path}")

  set(${compile_options_var}
      ${compile_options}
      PARENT_SCOPE)
endfunction()
