tylibs_build_get_property(target TYLIBS_TARGET)
tylibs_build_get_property(tylibs_path TYLIBS_PATH)

set(srcs)
set(includes "include" ${tylibs_path}/third_party/etl/include)
set(priv_includes "src/include")
list(
  APPEND
  srcs
  "src/instance/instance.cpp"
  "src/common/string.cpp"
  "src/common/error.cpp"
  "src/common/exit_code.c"
  "src/logging/logging.cpp"
  "src/logging/log.cpp")

if(${target} STREQUAL "linux")
  list(APPEND srcs "src/platform/linux/logging.c"
       "src/platform/linux/thread.cpp")
  list(APPEND priv_includes "src/platform/linux/")
elseif(${target} STREQUAL "esp")
  list(APPEND srcs "src/platform/esp/logging.c" "src/platform/esp/thread.c")
  list(APPEND priv_includes "src/platform/esp/")
endif()

tylibs_component_register(SRCS ${srcs} INCLUDE_DIRS ${includes}
                          PRIV_INCLUDE_DIRS ${priv_includes})
