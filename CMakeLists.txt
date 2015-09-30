#
#
#
#

set(PROJECT_NAME "libusp")

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

include(FindPkgConfig)
include(CMakePackageConfigHelpers)
pkg_search_module(UDEV REQUIRED libudev)
find_library(M_LIB m)

enable_testing()

set(LIBUSP_LIB "usp")
set(LIBUSP_INCLUDE "${PROJECT_SOURCE_DIR}/include")
set(LIBUSP_SRC "${PROJECT_SOURCE_DIR}/src")
set(LIBUSP_TEST "${PROJECT_SOURCE_DIR}/test")
set(LIBUSP_VERSION_MAJOR "0")
set(LIBUSP_VERSION_MINOR "0")
set(LIBUSP_VERSION_PATCH "1")
set(LIBUSP_VERSION
  "${LIBUSP_VERSION_MAJOR}.${LIBUSP_VERSION_MINOR}.${LIBUSP_VERSION_PATCH}")

# Uncomment to make makefiles verbose
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Compiler Options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3")

# Source
add_subdirectory(${LIBUSP_SRC})
add_subdirectory(${LIBUSP_INCLUDE})

# Includes
include_directories(${LIBUSP_INCLUDE})
include_directories(${UDEV_INCLUDE_DIRS})

# Library
add_library(${LIBUSP_LIB} SHARED ${SOURCE_FILES})
target_link_libraries(${LIBUSP_LIB} ${UDEV_LIBRARIES} ${M_LIB})

# Tests
add_subdirectory(${LIBUSP_TEST})

# Installation
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_INCLUDEDIR include)

# Config File Settings
set(LIBUSP_INSTALL_PREFIX      "${CMAKE_INSTALL_PREFIX}")
set(LIBUSP_INSTALL_EXEC_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(LIBUSP_INSTALL_LIBDIR
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(LIBUSP_INSTALL_INCLUDEDIR
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# Configure Files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libusp.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/libusp.pc @ONLY)

# pkg-config
set(PKGCONFIG_INSTALL_DIR lib/pkgconfig CACHE PATH
  "Installation directory for pkg-config files")

# Set install targets
install(TARGETS ${LIBUSP_LIBRARY}
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADER_FILES}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libusp.pc
  DESTINATION "${PKGCONFIG_INSTALL_DIR}")
