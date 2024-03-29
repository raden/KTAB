# ===========================================
# Copyright KAPSARC. Open Source MIT License 
# ===========================================

project(agenda)

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0017 NEW) # prefer built-in modules

set (LOCAL_MODULE_DIR
  ${PROJECT_SOURCE_DIR}/../../KTAB/cmakemodules)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${LOCAL_MODULE_DIR}
  )


set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_OUTPUT_PATH      ${PROJECT_SOURCE_DIR}/)
set(EXECUTABLE_OUTPUT_PATH   ${PROJECT_SOURCE_DIR}/)

if (UNIX)
  set (ENABLE_EFFCPP true CACHE  BOOL "Check Effective C++ Guidelines")
endif(UNIX) 

# -------------------------------------------------
set (ENABLE_EFENCE false CACHE  BOOL "Use Electric Fence memory debugger")

if (ENABLE_EFENCE)
  find_package(efence)
  if (NOT EFENCE_FOUND)
    message(FATAL_ERROR "Could not find Electric Fence memory debugger")
  endif (NOT EFENCE_FOUND)
endif (ENABLE_EFENCE)

# --------------------------------------------------------
find_package(Sqlite)
if (NOT SQLITE_FOUND)
  message(FATAL_ERROR "Could not find SQLite")
endif (NOT SQLITE_FOUND)

# --------------------------------------------------------
# See "Findkutils.cmake" in cmakemodules to figure
# out where it looks and what variables it sets
find_package(kutils)
if(NOT KUTILS_FOUND)
  message(FATAL_ERROR "Could not find kutils")
endif(NOT KUTILS_FOUND)


# Ditto for "Findkmodel.cmake"
find_package(kmodel)
if(NOT KMODEL_FOUND)
  message(FATAL_ERROR "Could not find kmodel")
endif(NOT KMODEL_FOUND)
 
 
# -------------------------------------------------
# demo program

INCLUDE_DIRECTORIES( 
  ${KUTILS_INCLUDE_DIR}
  ${KMODEL_INCLUDE_DIR}
  ${SQLITE_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/libsrc/
  ${PROJECT_SOURCE_DIR}/src/ 
  )
  
set(AGENDALIB_SRCS
  ${PROJECT_SOURCE_DIR}/libsrc/agenda.cpp 
  )

add_library(agenda STATIC ${AGENDALIB_SRCS})
 
ADD_EXECUTABLE(agdemo
  src/demo.cpp 
  )
 
target_link_libraries (agdemo
  agenda
  ${KMODEL_LIBRARY}
  ${KUTILS_LIBRARY}
  ${SQLITE_LIBRARIES}
  ${EFENCE_LIBRARIES}
  )
# -------------------------------------------------
# show some useful status/debugging information
message(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR}) 
# -------------------------------------------------
# As of February 2014, C++11 is still not the default
# for g++, so I have to provide it here.
# Also on Unix, the C++11 thread library relies on pthreads
if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 ")
  if (ENABLE_EFFCPP)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  endif (ENABLE_EFFCPP)
endif(UNIX)

# ===========================================
# Copyright KAPSARC. Open Source MIT License 
# ===========================================