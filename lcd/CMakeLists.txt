if (NOT DEFINED IS_TESTING)

set(LIB_NAME display)

set(${LIB_NAME}_SRCS 
  ${CMAKE_CURRENT_SOURCE_DIR}/glcd.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/stlcd.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/display.c
  ${CMAKE_CURRENT_SOURCE_DIR}/fonts.c PARENT_SCOPE)

set(${LIB_NAME}_DIRS ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

elseif("Y" STREQUAL ${IS_TESTING})
cmake_minimum_required(VERSION 3.10)
project(test_display)

find_package(PkgConfig REQUIRED) 

pkg_check_modules(STB_PKG REQUIRED IMPORTED_TARGET stb)

add_executable(test_display glcd.c fonts.c test.c)
target_include_directories(test_display PUBLIC ../)
target_link_libraries(test_display PUBLIC PkgConfig::STB_PKG m)

endif()