#
#
#
#

set(PROJECT_NAME "libuspwm")

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

include(FindPkgConfig)
include(CMakePackageConfigHelpers)
pkg_search_module(UDEV REQUIRED libudev)

enable_testing()

set(USPWM_LIB "uspwm")
set(USPWM_INCLUDE "${PROJECT_SOURCE_DIR}/include")
set(USPWM_SRC "${PROJECT_SOURCE_DIR}/src")
set(USPWM_TEST "${PROJECT_SOURCE_DIR}/test")

# Uncomment to make makefiles verbose
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Compiler Options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3")

# Source 
add_subdirectory(${USPWM_SRC})
include_directories(${USPWM_INCLUDE})
include_directories(${UDEV_INCLUDE_DIR})

# Library
add_library(${USPWM_LIB} SHARED ${SOURCE_FILES})
target_link_libraries(${USPWM_LIB} ${UDEV_LIBRARY})

# Tests
add_subdirectory(${USPWM_TEST})

# Installation
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_INCLUDEDIR include)
