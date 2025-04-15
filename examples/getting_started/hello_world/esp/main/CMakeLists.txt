idf_component_register(SRCS "main.cpp")

# Include for TYLIBS build system functions
include($ENV{TYLIBS_PATH}/tools/cmake/tylibs.cmake)
# Create tylibs::common static libraries
tylibs_build_process(
  "esp"
  COMPONENTS
  common
  bus
  settings
  TYCONFIG
  ${CMAKE_CURRENT_LIST_DIR}/../../tyconfig
  TYCONFIG_DEFAULTS
  ${CMAKE_CURRENT_LIST_DIR}/../../tyconfig.defaults
  BUILD_DIR
  ${CMAKE_BINARY_DIR})

target_link_libraries(${COMPONENT_LIB} PRIVATE tylibs::common tylibs::bus
                                               tylibs::settings tyapp)
