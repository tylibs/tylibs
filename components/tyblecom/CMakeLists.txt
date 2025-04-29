tylibs_build_get_property(target TYLIBS_TARGET)

set(srcs)
set(includes "include")

if(CONFIG_TY_BLECOM)
  set(priv_includes "src/include")
  list(APPEND srcs "src/blecom.cpp")
  # "src/mqtt/mqtt.cpp")

  if(${target} STREQUAL "linux")
    list(APPEND srcs "src/platform/linux/blecom.linux.cpp")
    list(APPEND priv_includes "src/platform/linux/")
    # include MQTT library
    find_package(sdbus-c++ REQUIRED)
  elseif(${target} STREQUAL "esp")
    list(APPEND srcs "src/platform/esp/blecom.esp.cpp")
    list(APPEND priv_includes "src/platform/esp/")
  endif()
endif()

tylibs_component_register(
  SRCS
  ${srcs}
  INCLUDE_DIRS
  ${includes}
  PRIV_INCLUDE_DIRS
  ${priv_includes}
  PRIV_REQUIRES
  "tycommon")

if(CONFIG_TY_BLECOM)
  if(${target} STREQUAL "linux")
    target_link_libraries(${COMPONENT_LIB} PRIVATE SDBusCpp::sdbus-c++)
    # target_link_options(${COMPONENT_LIB} PRIVATE "-Wl,--print-memory-usage")
  elseif(${target} STREQUAL "esp")
    target_link_libraries(${COMPONENT_LIB} PRIVATE __idf_nvs_flash __idf_bt)
  endif()
endif()
