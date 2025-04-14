get_property(__idf_env_set GLOBAL PROPERTY __IDF_ENV_SET)
if(NOT __idf_env_set)
  # Infer an TYLIBS_PATH relative to the cmake directory
  get_filename_component(_tylibs_path "${CMAKE_CURRENT_LIST_DIR}/../.."
                         REALPATH)
  file(TO_CMAKE_PATH "${_tylibs_path}" _tylibs_path)

  # Get the path set in environment
  set(tylibs_path $ENV{TYLIBS_PATH})
  # Environment TYLIBS_PATH should match the inferred TYLIBS_PATH. If not, warn
  # the user. (Note: REALPATH is needed in both above steps to account for case
  # on case insensitive filesystems, or relative paths)
  if(tylibs_path)
    get_filename_component(tylibs_path "${tylibs_path}" REALPATH)
    file(TO_CMAKE_PATH "${tylibs_path}" tylibs_path)

    if(NOT tylibs_path STREQUAL _tylibs_path)
      message(
        WARNING
          "TYLIBS_PATH environment variable is different from inferred TYLIBS_PATH.
                            Check if your project's top-level CMakeLists.txt includes the right
                            CMake files. Environment TYLIBS_PATH will be used for the build:
                            ${tylibs_path}")
    endif()
  else()
    message(
      WARNING
        "TYLIBS_PATH environment variable not found. Setting TYLIBS_PATH to '${_tylibs_path}'."
    )
    set(tylibs_path ${_tylibs_path})
    set(ENV{TYLIBS_PATH} ${_tylibs_path})
  endif()
  # Include other CMake modules required
  set(CMAKE_MODULE_PATH
      "${tylibs_path}/tools/cmake" "${tylibs_path}/tools/cmake/third_party"
      ${CMAKE_MODULE_PATH})

  include(build)

  set(TYLIBS_PATH ${tylibs_path})

  include(utilities)
  include(GetGitRevisionDescription)
  include(CheckCCompilerFlag)
  include(CheckCXXCompilerFlag)
  include(git_submodules)
  include(ldgen)
  include(prefix_map)
  include(depgraph)
  include(kconfig)
  include(component)

  __build_init("${tylibs_path}")

  set_property(GLOBAL PROPERTY __TYLIBS_ENV_SET 1)
endif()
