tylibs_build_get_property(target TYLIBS_TARGET)

set(srcs)
set(includes "include")

if(CONFIG_TYNET)
  set(priv_includes "src/include")
  list(APPEND srcs "src/connection.cpp" "src/network_interface.cpp"
       "src/mqtt/mqtt.cpp")

  if(${target} STREQUAL "linux")
    list(APPEND srcs "src/platform/linux/mqtt.linux.cpp")
    list(APPEND priv_includes "src/platform/linux/")
    # include MQTT library
    find_package(PahoMqttCpp REQUIRED)
  elseif(${target} STREQUAL "esp")
    list(APPEND srcs "src/platform/esp/mqtt.esp.cpp")
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
  "common")

if(${target} STREQUAL "linux")
  target_link_libraries(${COMPONENT_LIB} PRIVATE PahoMqttCpp::paho-mqttpp3)
endif()
