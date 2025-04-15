# SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${TYLIBS_TARGET} STREQUAL "linux")
  include($ENV{TYLIBS_PATH}/tools/cmake/ty-project.cmake)
  list(APPEND EXTRA_COMPONENT_DIRS "linux")
elseif(${TYLIBS_TARGET} STREQUAL "esp")
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)
  idf_build_set_property(MINIMAL_BUILD ON)
  list(APPEND EXTRA_COMPONENT_DIRS "esp")
else()
  message(ERROR_FATAL "no valid TYLIBS_TARGET given")
endif()

project(hello_world)

add_subdirectory(src)
