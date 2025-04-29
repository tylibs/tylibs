if(${TYLIBS_TARGET} STREQUAL "linux")
  include($ENV{TYLIBS_PATH}/tools/cmake/ty-project.cmake)
  list(APPEND EXTRA_COMPONENT_DIRS "linux")
  # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}
  # -Wl,--print-memory-usage")
elseif(${TYLIBS_TARGET} STREQUAL "esp")
  set(SDKCONFIG_DEFAULTS
      "$ENV{TYLIBS_PATH}/sdkconfig.esp32;${CMAKE_SOURCE_DIR}/sdkconfig.defaults"
  )
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)
  idf_build_set_property(MINIMAL_BUILD ON)
  list(APPEND EXTRA_COMPONENT_DIRS "esp")
  # add this global sdkconfig
  idf_build_get_property(test SDKCONFIG_DEFAULTS)
  message(STATUS "SDKCONFIG_DEFAULTS: ${test}")
else()
  message(ERROR_FATAL "no valid TYLIBS_TARGET given")
endif()
