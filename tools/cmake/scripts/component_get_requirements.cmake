# if(IN_LIST) is used, which requires CMP0057 set to NEW
cmake_policy(SET CMP0057 NEW)
include("${BUILD_PROPERTIES_FILE}")
include("${COMPONENT_PROPERTIES_FILE}")

function(tylibs_build_get_property var property)
  cmake_parse_arguments(_ "GENERATOR_EXPRESSION" "" "" ${ARGN})
  if(__GENERATOR_EXPRESSION)
    message(FATAL_ERROR "Getting build property generator expression not
                supported before tylibs_component_register().")
  endif()
  set(${var}
      ${${property}}
      PARENT_SCOPE)
endfunction()

tylibs_build_get_property(tylibs_path TYLIBS_PATH)
include(${tylibs_path}/tools/cmake/ty-utilities.cmake)
include(${tylibs_path}/tools/cmake/ty-version.cmake)

function(__ty_component_get_property var component_target property)
  set(_property __ty_component_${component_target}_${property})
  set(${var}
      ${${_property}}
      PARENT_SCOPE)
endfunction()

#
# Given a component name or alias, get the corresponding component target.
#
function(__ty_component_get_target var name_or_alias)
  tylibs_build_get_property(component_targets __TY_COMPONENT_TARGETS)

  # Assume first that the parameters is an alias.
  string(REPLACE "::" "_" name_or_alias "${name_or_alias}")
  set(component_target ___${name_or_alias})

  if(component_target IN_LIST component_targets)
    set(${var}
        ${component_target}
        PARENT_SCOPE)
    set(target ${component_target})
  else() # assumption is wrong, try to look for it manually
    unset(target)
    foreach(component_target ${component_targets})
      __ty_component_get_property(_component_name ${component_target}
                                  COMPONENT_NAME)
      if(name_or_alias STREQUAL _component_name)
        set(target ${component_target})
        break()
      endif()
    endforeach()
    set(${var}
        ${target}
        PARENT_SCOPE)
  endif()
endfunction()

function(tylibs_component_get_property var component property)
  __ty_component_get_target(component_target ${component})
  __ty_component_get_property(_var ${component_target} ${property})
  set(${var}
      ${_var}
      PARENT_SCOPE)
endfunction()

macro(require_tylibs_targets)

endmacro()

macro(tylibs_component_mock)
  set(options)
  set(single_value)
  set(multi_value MOCK_HEADER_FILES INCLUDE_DIRS REQUIRES)
  cmake_parse_arguments(_ "${options}" "${single_value}" "${multi_value}"
                        "${ARGN}")
  tylibs_component_register(REQUIRES ${__REQUIRES} cmock)
  return()
endmacro()

macro(tylibs_component_register)
  set(options WHOLE_ARCHIVE)
  set(single_value KCONFIG KCONFIG_PROJBUILD)
  set(multi_value
      SRCS
      SRC_DIRS
      EXCLUDE_SRCS
      INCLUDE_DIRS
      PRIV_INCLUDE_DIRS
      LDFRAGMENTS
      REQUIRES
      PRIV_REQUIRES
      REQUIRED_TYLIBS_TARGETS
      EMBED_FILES
      EMBED_TXTFILES)
  cmake_parse_arguments(_ "${options}" "${single_value}" "${multi_value}"
                        "${ARGN}")
  set(__ty_component_priv_requires "${__PRIV_REQUIRES}")
  set(__ty_component_requires "${__REQUIRES}")
  set(__ty_component_kconfig "${__KCONFIG}")
  set(__ty_component_kconfig_projbuild "${__KCONFIG_PROJBUILD}")
  set(__ty_component_include_dirs "${__INCLUDE_DIRS}")
  set(__ty_component_registered 1)
  return()
endmacro()

macro(register_component)
  set(__ty_component_requires "${COMPONENT_REQUIRES}")
  set(__ty_component_priv_requires "${COMPONENT_PRIV_REQUIRES}")
  set(__ty_component_registered 1)
  return()
endmacro()

macro(register_config_only_component)
  register_component()
endmacro()

tylibs_build_get_property(__common_reqs __TY_COMPONENT_REQUIRES_COMMON)
tylibs_build_get_property(__ty_component_targets __TY_COMPONENT_TARGETS)

function(__ty_component_get_requirements)
  # This is in a function (separate variable context) so that variables declared
  # and set by the included CMakeLists.txt does not bleed into the next
  # inclusion. We are only interested in the public and private requirements of
  # components
  __ty_component_get_property(__ty_component_dir ${__ty_component_target}
                              COMPONENT_DIR)
  __ty_component_get_property(__ty_component_name ${__ty_component_target}
                              COMPONENT_NAME)
  set(COMPONENT_NAME ${__ty_component_name})
  set(COMPONENT_DIR ${__ty_component_dir})
  # COMPONENT_DIR is preferred
  include(${__ty_component_dir}/CMakeLists.txt OPTIONAL)

  spaces2list(__ty_component_requires)
  spaces2list(__ty_component_priv_requires)
  spaces2list(__ty_component_include_dirs)

  set(__ty_component_requires
      "${__ty_component_requires}"
      PARENT_SCOPE)
  set(__ty_component_priv_requires
      "${__ty_component_priv_requires}"
      PARENT_SCOPE)
  set(__ty_component_kconfig
      "${__ty_component_kconfig}"
      PARENT_SCOPE)
  set(__ty_component_kconfig_projbuild
      "${__ty_component_kconfig_projbuild}"
      PARENT_SCOPE)
  set(__ty_component_include_dirs
      "${__ty_component_include_dirs}"
      PARENT_SCOPE)
  set(__ty_component_registered
      ${__ty_component_registered}
      PARENT_SCOPE)
endfunction()

set(CMAKE_BUILD_EARLY_EXPANSION 1)

# smaller number means lower priority
set(__TARGETS_TYLIBS_COMPONENTS "") # 0
set(__TARGETS_PROJECT_MANAGED_COMPONENTS "") # 1
set(__TARGETS_PROJECT_EXTRA_COMPONENTS "") # 2
set(__TARGETS_PROJECT_COMPONENTS "") # 3

foreach(__ty_component_target ${__ty_component_targets})
  __ty_component_get_property(__ty_component_source ${__ty_component_target}
                              COMPONENT_SOURCE)

  if("${__ty_component_source}" STREQUAL "tylibs_components")
    list(APPEND __TARGETS_TYLIBS_COMPONENTS ${__ty_component_target})
  elseif("${__ty_component_source}" STREQUAL "project_managed_components")
    list(APPEND __TARGETS_PROJECT_MANAGED_COMPONENTS ${__ty_component_target})
  elseif("${__ty_component_source}" STREQUAL "project_extra_components")
    list(APPEND __TARGETS_PROJECT_EXTRA_COMPONENTS ${__ty_component_target})
  elseif("${__ty_component_source}" STREQUAL "project_components")
    list(APPEND __TARGETS_PROJECT_COMPONENTS ${__ty_component_target})
  else()
    message(
      FATAL_ERROR
        "Unknown component source for component ${__ty_component_target}: ${__ty_component_source}"
    )
  endif()
endforeach()

# priority higher ones goes first
set(__sorted_component_targets "")
foreach(__target IN
        LISTS __TARGETS_PROJECT_COMPONENTS __TARGETS_PROJECT_EXTRA_COMPONENTS
              __TARGETS_PROJECT_MANAGED_COMPONENTS __TARGETS_TYLIBS_COMPONENTS)
  __ty_component_get_property(__ty_component_name ${__target} COMPONENT_NAME)
  list(APPEND __sorted_component_targets ${__target})
endforeach()

foreach(__ty_component_target ${__sorted_component_targets})
  set(__ty_component_requires "")
  set(__ty_component_priv_requires "")
  set(__ty_component_registered 0)

  __ty_component_get_requirements()

  # Remove duplicates and the component itself from its requirements
  __ty_component_get_property(__ty_component_alias ${__ty_component_target}
                              COMPONENT_ALIAS)
  __ty_component_get_property(__ty_component_name ${__ty_component_target}
                              COMPONENT_NAME)

  # Prevent component from linking to itself.
  if(__ty_component_requires)
    list(REMOVE_DUPLICATES __ty_component_requires)
    list(REMOVE_ITEM __ty_component_requires ${__ty_component_alias}
         ${__ty_component_name})
  endif()

  if(__ty_component_priv_requires)
    list(REMOVE_DUPLICATES __ty_component_priv_requires)
    list(REMOVE_ITEM __ty_component_priv_requires ${__ty_component_alias}
         ${__ty_component_name})
  endif()

  __ty_component_get_property(__ty_component_source ${__ty_component_target}
                              COMPONENT_SOURCE)

  set(__contents
      "__ty_component_set_property(${__ty_component_target} REQUIRES \"${__ty_component_requires}\")
__ty_component_set_property(${__ty_component_target} PRIV_REQUIRES \"${__ty_component_priv_requires}\")
__ty_component_set_property(${__ty_component_target} __TY_COMPONENT_REGISTERED ${__ty_component_registered})
__ty_component_set_property(${__ty_component_target} INCLUDE_DIRS \"${__ty_component_include_dirs}\")
__ty_component_set_property(${__ty_component_target} __TY_COMPONENT_SOURCE \"${__ty_component_source}\")"
  )

  if(__ty_component_kconfig)
    get_filename_component(__ty_component_kconfig "${__ty_component_kconfig}"
                           ABSOLUTE)
    set(__contents
        "${__contents}\n__ty_component_set_property(${__ty_component_target} KCONFIG \"${__ty_component_kconfig}\")"
    )
  endif()

  if(__ty_component_kconfig_projbuild)
    get_filename_component(__ty_component_kconfig "${__ty_component_kconfig}"
                           ABSOLUTE)
    set(__contents
        "${__contents}\n__ty_component_set_property(${__ty_component_target} KCONFIG_PROJBUILD \"${__ty_component_kconfig_projbuild}\")"
    )
  endif()

  set(__ty_component_requires_contents
      "${__ty_component_requires_contents}\n${__contents}")
endforeach()

file(WRITE ${COMPONENT_REQUIRES_FILE} "${__ty_component_requires_contents}")
