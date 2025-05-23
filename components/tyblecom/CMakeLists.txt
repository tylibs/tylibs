tylibs_build_get_property(target TYLIBS_TARGET)

set(srcs)
set(includes "include")

if(CONFIG_TY_BLECOM)
  set(priv_includes "src/include")
  list(APPEND srcs "src/blecom.cpp")
  # "src/mqtt/mqtt.cpp")

  if(${target} STREQUAL "linux")
    find_package(sdbus-c++ REQUIRED)
    find_package(sdbus-c++-tools REQUIRED)
    FetchContent_Declare(
      fmt
      GIT_REPOSITORY https://github.com/fmtlib/fmt.git
      GIT_TAG 11.2.0)
    FetchContent_MakeAvailable(fmt)
    add_custom_command(
      OUTPUT
        "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/tyblecom-proxy-glue.h"
      COMMAND
        SDBusCpp::sdbus-c++-xml2cpp
        "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/dbus/proxy.xml"
        --proxy="${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/tyblecom-proxy-glue.h"
      COMMENT "Generating D-Bus interfaces.")
    add_custom_command(
      OUTPUT
        "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/tyblecom-adaptor-glue.h"
      COMMAND
        SDBusCpp::sdbus-c++-xml2cpp
        "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/dbus/adapter.xml"
        --adaptor="${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/tyblecom-adaptor-glue.h"
      COMMENT "Generating D-Bus interfaces.")
    list(APPEND priv_includes "src/platform/linux/")
    list(
      APPEND
      srcs
      "src/platform/linux/blecom.linux.cpp"
      "src/platform/linux/gatt_characteristic1.cpp"
      "src/platform/linux/gatt_service1.cpp"
      "src/platform/linux/gatt_descriptor1.cpp"
      "src/platform/linux/util.cpp"
      "src/platform/linux/tyblecom-proxy-glue.h"
      "src/platform/linux/tyblecom-adaptor-glue.h")
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
    target_link_libraries(${COMPONENT_LIB} PRIVATE SDBusCpp::sdbus-c++ fmt::fmt)
    # target_link_options(${COMPONENT_LIB} PRIVATE "-Wl,--print-memory-usage")
  elseif(${target} STREQUAL "esp")
    target_link_libraries(${COMPONENT_LIB} PRIVATE __idf_nvs_flash __idf_bt
                                                   __idf_esp-nimble-cpp)
  endif()
endif()
