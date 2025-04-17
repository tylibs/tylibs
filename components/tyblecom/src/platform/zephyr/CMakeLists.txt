cmake_minimum_required(VERSION 3.20)

ty_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/settings.c)

ty_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ty_library_compile_definitions(
  -DTY_NET_CONFIG_LOG_LEVEL=${CONFIG_TY_NET_LOG_LEVEL}
  -DTY_NET_CONFIG_FILE="tynet-zephyr-config.h")
