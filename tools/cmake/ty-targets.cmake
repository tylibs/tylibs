#
# Get target from single tyconfig file
#
function(__target_from_config config target_out file_out)
  set(${target_out}
      NOTFOUND
      PARENT_SCOPE)
  set(${file_out}
      NOTFOUND
      PARENT_SCOPE)

  if(NOT EXISTS "${config}")
    return()
  endif()

  file(STRINGS "${config}" lines)
  foreach(line ${lines})
    if(NOT "${line}" MATCHES "^CONFIG_IDF_TARGET=\"[^\"]+\"$")
      continue()
    endif()

    string(REGEX REPLACE "CONFIG_IDF_TARGET=\"([^\"]+)\"" "\\1" target
                         "${line}")
    set(${target_out}
        ${target}
        PARENT_SCOPE)
    set(${file_out}
        ${config}
        PARENT_SCOPE)
    return()
  endforeach()
endfunction()

#
# Get target from list of tyconfig files
#
function(__target_from_configs configs target_out file_out)
  set(target NOTFOUND)
  set(file NOTFOUND)

  foreach(config ${configs})
    message(DEBUG "Searching for target in '${config}'")
    get_filename_component(config "${config}" ABSOLUTE)
    __target_from_config("${config}" target file)
    if(target)
      break()
    endif()
  endforeach()

  set(${target_out}
      ${target}
      PARENT_SCOPE)
  set(${file_out}
      ${file}
      PARENT_SCOPE)
endfunction()

#
# Search for target in config files in the following order. TYCONFIG cmake var,
# default tyconfig, TYCONFIG_DEFAULTS cmake var if non-empty or
# TYCONFIG_DEFAULTS env var if non-empty or tyconfig.defaults.
#
function(__target_guess target_out file_out)
  # Select tyconfig_defaults to look for target
  if(TYCONFIG_DEFAULTS)
    set(defaults "${TYCONFIG_DEFAULTS}")
  elseif(DEFINED ENV{TYCONFIG_DEFAULTS})
    set(defaults "$ENV{TYCONFIG_DEFAULTS}")
  endif()

  if(NOT defaults)
    set(defaults "${CMAKE_SOURCE_DIR}/tyconfig.defaults")
  endif()

  set(configs "${TYCONFIG}" "${CMAKE_SOURCE_DIR}/tyconfig" "${defaults}")
  message(DEBUG "Searching for target in '${configs}'")
  __target_from_configs("${configs}" target file)
  set(${target_out}
      ${target}
      PARENT_SCOPE)
  set(${file_out}
      ${file}
      PARENT_SCOPE)
endfunction()

#
# Set the target used for the standard project build.
#
macro(__target_init config_file)
  # Input is IDF_TARGET environement variable
  set(env_idf_target $ENV{IDF_TARGET})

  if(NOT env_idf_target)
    # IDF_TARGET not set in environment, see if it is set in cache
    if(IDF_TARGET)
      set(env_idf_target ${IDF_TARGET})
    else()
      # Try to guess IDF_TARGET from tyconfig files while honoring TYCONFIG and
      # TYCONFIG_DEFAULTS values
      __target_guess(env_idf_target where)
      if(env_idf_target)
        message(
          STATUS
            "IDF_TARGET is not set, guessed '${env_idf_target}' from tyconfig '${where}'"
        )
      else()
        set(env_idf_target esp32)
        message(
          STATUS "IDF_TARGET not set, using default target: ${env_idf_target}")
      endif()
    endif()
  endif()

  # Check if selected target is consistent with CMake cache
  if(DEFINED CACHE{IDF_TARGET})
    if(NOT $CACHE{IDF_TARGET} STREQUAL ${env_idf_target})
      message(
        FATAL_ERROR
          " IDF_TARGET '$CACHE{IDF_TARGET}' in CMake"
          " cache does not match currently selected IDF_TARGET '${env_idf_target}'."
          " To change the target, clear the build directory and tyconfig file,"
          " and build the project again.")
    endif()
  endif()

  # Check if selected target is consistent with tyconfig
  __target_from_config("${config_file}" tyconfig_target where)
  if(tyconfig_target)
    if(NOT ${tyconfig_target} STREQUAL ${env_idf_target})
      message(
        FATAL_ERROR
          " Target '${tyconfig_target}' in tyconfig '${where}'"
          " does not match currently selected IDF_TARGET '${IDF_TARGET}'."
          " To change the target, clear the build directory and tyconfig file,"
          " and build the project again.")
    endif()
  endif()

  # IDF_TARGET will be used by component manager, make sure it is set
  set(ENV{IDF_TARGET} ${env_idf_target})

  # Finally, set IDF_TARGET in cache
  set(IDF_TARGET
      ${env_idf_target}
      CACHE STRING "IDF Build Target")
endmacro()

#
# Used by the project CMake file to set the toolchain before project() call.
#
macro(__target_set_toolchain)
  idf_build_get_property(idf_path IDF_PATH)

  # See if Clang toolchain should be used
  set(env_idf_toolchain $ENV{IDF_TOOLCHAIN})
  if(NOT env_idf_toolchain)
    # IDF_TOOLCHAIN not set in environment, see if it is set in cache
    if(IDF_TOOLCHAIN)
      set(env_idf_toolchain ${IDF_TOOLCHAIN})
    else()
      set(env_idf_toolchain gcc)
    endif()
  elseif(DEFINED CACHE{IDF_TOOLCHAIN})
    # IDF_TOOLCHAIN set both in environment and in cache, must be the same
    if(NOT $CACHE{IDF_TOOLCHAIN} STREQUAL ${env_idf_toolchain})
      message(
        FATAL_ERROR
          " IDF_TOOLCHAIN '$CACHE{IDF_TOOLCHAIN}' in CMake cache does not match"
          " currently selected IDF_TOOLCHAIN '${env_idf_toolchain}'. To change the toolchain, clear"
          " the build directory and tyconfig file, and build the project again."
      )
    endif()
  endif()

  # Finally, set IDF_TOOLCHAIN in cache
  set(IDF_TOOLCHAIN
      ${env_idf_toolchain}
      CACHE STRING "IDF Build Toolchain Type")

  if(${env_idf_toolchain} STREQUAL "clang")
    set(toolchain_type "clang-")
  endif()

  # Check if selected target is consistent with toolchain file in CMake cache
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    string(FIND "${CMAKE_TOOLCHAIN_FILE}"
                "-${toolchain_type}${IDF_TARGET}.cmake" found)
    if(${found} EQUAL -1)
      get_filename_component(toolchain "${CMAKE_TOOLCHAIN_FILE}" NAME_WE)
      message(
        FATAL_ERROR
          " CMAKE_TOOLCHAIN_FILE '${toolchain}'"
          " does not match currently selected IDF_TARGET '${IDF_TARGET}'."
          " To change the target, clear the build directory and tyconfig file,"
          " and build the project again.")
    endif()
  endif()

  # First try to load the toolchain file from the tools/cmake/directory of IDF
  set(toolchain_file_global
      ${tylibs_path}/tools/cmake/toolchain-${ty-toolchain_type}${IDF_TARGET}.cmake
  )
  if(EXISTS ${toolchain_file_global})
    set(CMAKE_TOOLCHAIN_FILE ${toolchain_file_global})
  else()
    message(FATAL_ERROR "Toolchain file ${toolchain_file_global} not found")
  endif()
endmacro()

#
# Set the target used for the standard project build.
#
macro(__target_init)
  # Input is TYLIBS_TARGET environement variable
  set(env_tylibs_target $ENV{TYLIBS_TARGET})

  if(NOT env_tylibs_target)
    # TYLIBS_TARGET not set in environment, see if it is set in cache
    if(TYLIBS_TARGET)
      set(env_tylibs_target ${TYLIBS_TARGET})
    else()
      set(env_tylibs_target linux)
      message(
        STATUS
          "TYLIBS_TARGET not set, using default target: ${env_tylibs_target}")
    endif()
  endif()

  # Check if selected target is consistent with CMake cache
  if(DEFINED CACHE{TYLIBS_TARGET})
    if(NOT $CACHE{TYLIBS_TARGET} STREQUAL ${env_tylibs_target})
      message(
        FATAL_ERROR
          " TYLIBS_TARGET '$CACHE{TYLIBS_TARGET}' in CMake"
          " cache does not match currently selected TYLIBS_TARGET '${env_tylibs_target}'."
      )
    endif()
  endif()

  # TYLIBS_TARGET will be used by component manager, make sure it is set
  set(ENV{TYLIBS_TARGET} ${env_tylibs_target})

  # Finally, set TYLIBS_TARGET in cache
  set(TYLIBS_TARGET
      ${env_tylibs_target}
      CACHE STRING "TYLIBS Build Target")
endmacro()

#
# Used by the project CMake file to set the toolchain before project() call.
#
macro(__target_set_toolchain)
  tylibs_build_get_property(tylibs_path TYLIBS_PATH)

  # See if Clang toolchain should be used
  set(env_tylibs_toolchain $ENV{TYLIBS_TOOLCHAIN})
  if(NOT env_tylibs_toolchain)
    # TYLIBS_TOOLCHAIN not set in environment, see if it is set in cache
    if(TYLIBS_TOOLCHAIN)
      set(env_tylibs_toolchain ${TYLIBS_TOOLCHAIN})
    else()
      set(env_tylibs_toolchain gcc)
    endif()
  elseif(DEFINED CACHE{TYLIBS_TOOLCHAIN})
    # TYLIBS_TOOLCHAIN set both in environment and in cache, must be the same
    if(NOT $CACHE{TYLIBS_TOOLCHAIN} STREQUAL ${env_tylibs_toolchain})
      message(
        FATAL_ERROR
          " TYLIBS_TOOLCHAIN '$CACHE{TYLIBS_TOOLCHAIN}' in CMake cache does not match"
          " currently selected TYLIBS_TOOLCHAIN '${env_tylibs_toolchain}'. To change the toolchain, clear"
          " the build directory and tyconfig file, and build the project again."
      )
    endif()
  endif()

  # Finally, set TYLIBS_TOOLCHAIN in cache
  set(TYLIBS_TOOLCHAIN
      ${env_tylibs_toolchain}
      CACHE STRING "TYLIBS Build Toolchain Type")

  if(${env_tylibs_toolchain} STREQUAL "clang")
    set(toolchain_type "clang-")
  endif()

  # Check if selected target is consistent with toolchain file in CMake cache
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    string(FIND "${CMAKE_TOOLCHAIN_FILE}"
                "-${toolchain_type}${TYLIBS_TARGET}.cmake" found)
    if(${found} EQUAL -1)
      get_filename_component(toolchain "${CMAKE_TOOLCHAIN_FILE}" NAME_WE)
      message(
        FATAL_ERROR
          " CMAKE_TOOLCHAIN_FILE '${toolchain}'"
          " does not match currently selected TYLIBS_TARGET '${TYLIBS_TARGET}'."
          " To change the target, clear the build directory and tyconfig file,"
          " and build the project again.")
    endif()
  endif()

  # First try to load the toolchain file from the tools/cmake/directory of
  # TYLIBS
  set(toolchain_file_global
      ${tylibs_path}/tools/cmake/ty-toolchain-${toolchain_type}${TYLIBS_TARGET}.cmake
  )
  if(EXISTS ${toolchain_file_global})
    set(CMAKE_TOOLCHAIN_FILE ${toolchain_file_global})
  else()
    message(FATAL_ERROR "Toolchain file ${toolchain_file_global} not found")
  endif()
endmacro()
