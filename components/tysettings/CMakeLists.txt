tylibs_build_get_property(target TYLIBS_TARGET)

set(srcs)
set(priv_includes)
set(includes "include")
set(priv_requires "tycommon")

if(CONFIG_TY_SETTINGS)
  if(${target} STREQUAL "linux")
    list(APPEND srcs "src/platform/linux/settings.cpp"
         "src/platform/linux/settings_file.cpp")
    list(APPEND priv_includes "src/platform/linux/")
  elseif(${target} STREQUAL "esp")
    list(APPEND srcs "src/platform/esp/settings.c")
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
  ${priv_requires})

if(CONFIG_TY_SETTINGS)
  if(${target} STREQUAL "esp")
    target_link_libraries(${COMPONENT_LIB} PRIVATE __idf_nvs_flash)
  endif()
endif()
