cmake_minimum_required(VERSION 3.16)
project(tylibs C CXX ASM)
if(CMAKE_CURRENT_LIST_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(
    FATAL_ERROR
      "Current directory '${CMAKE_CURRENT_LIST_DIR}' is not buildable. "
      "Change directories to one of the example projects in '${CMAKE_CURRENT_LIST_DIR}/examples' and try "
      "again.")
endif()

include(FetchContent)

tylibs_build_set_property(COMPILE_OPTIONS "${compile_options}" APPEND)
tylibs_build_set_property(C_COMPILE_OPTIONS "${c_compile_options}" APPEND)
tylibs_build_set_property(CXX_COMPILE_OPTIONS "${cxx_compile_options}" APPEND)
tylibs_build_set_property(ASM_COMPILE_OPTIONS "${asm_compile_options}" APPEND)
tylibs_build_set_property(COMPILE_DEFINITIONS "${compile_definitions}" APPEND)
tylibs_build_set_property(LINK_OPTIONS "${link_options}" APPEND)

tylibs_build_get_property(build_component_targets __TY_BUILD_COMPONENT_TARGETS)
# Add each component as a subdirectory, processing each component's
# CMakeLists.txt
foreach(component_target ${build_component_targets})
  __ty_component_get_property(dir ${component_target} COMPONENT_DIR)
  __ty_component_get_property(_name ${component_target} COMPONENT_NAME)
  __ty_component_get_property(prefix ${component_target} __PREFIX)
  __ty_component_get_property(alias ${component_target} COMPONENT_ALIAS)
  set(COMPONENT_NAME ${_name})
  set(COMPONENT_DIR ${dir})
  set(COMPONENT_ALIAS ${alias})
  tylibs_build_get_property(build_prefix __PREFIX)
  set(__tylibs_component_context 1)
  if(NOT prefix STREQUAL build_prefix)
    add_subdirectory(${dir} ${prefix}_${_name})
  else()
    add_subdirectory(${dir} ${_name})
  endif()
  set(__tylibs_component_context 0)
endforeach()
