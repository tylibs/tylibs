# SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{TYLIBS_PATH}/tools/cmake/project.cmake)

project(blecom)

add_subdirectory(src)
