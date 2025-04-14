idf_component_register(SRCS "main.cpp")

message(
  STATUS "CMakeLists.txt: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_LIST_DIR}")

# Include for TYLIBS build system functions
include($ENV{TYLIBS_PATH}/tools/cmake/tylibs.cmake)
# Create tylibs::common static libraries
tylibs_build_process(
  "esp"
  COMPONENTS
  common
  TYCONFIG
  ${CMAKE_CURRENT_LIST_DIR}/../tyconfig
  BUILD_DIR
  ${CMAKE_BINARY_DIR})

target_link_libraries(${COMPONENT_LIB} PRIVATE tylibs::common)
