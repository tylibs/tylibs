tylibs_build_get_property(target TYLIBS_TARGET)

set(srcs)
set(includes "include")

if(CONFIG_TYBUS)
  set(priv_includes "src/include")
  list(APPEND srcs "src/tybus.cpp")

  if(${target} STREQUAL "linux")
    list(APPEND srcs "src/platform/linux/scheduler.cpp"
         "src/platform/linux/threadqueue.c")
    list(APPEND priv_includes "src/platform/linux/")
  elseif(${target} STREQUAL "esp")
    list(APPEND srcs "src/platform/esp/scheduler.cpp")
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
