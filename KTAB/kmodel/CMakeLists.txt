# --------------------------------------------
# Copyright KAPSARC. Open source MIT License.
# --------------------------------------------
# The MIT License (MIT)
# 
# Copyright (c) 2015 King Abdullah Petroleum Studies and Research Center
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
# the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# -------------------------------------------------

project(kmodel)

cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/../cmakemodules)

set(LIBRARY_OUTPUT_PATH      ${PROJECT_SOURCE_DIR}/)
set(EXECUTABLE_OUTPUT_PATH   ${PROJECT_SOURCE_DIR}/)

# -------------------------------------------------
# basic check that this is a supported compiler/OS
# -------------------------------------------------
if (WIN32)
  message (STATUS "OS: WIN32 (generic Windows)")
  if (MSVC)
    message (STATUS "Compiler: MSVC")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    message (STATUS "Compiler: GCC")
  else(MSVC)
    message(SEND_ERROR "Unsupported Windows compiler")
  endif()
elseif(UNIX)
  message (STATUS "OS: UNIX (generic *nix)")
else()
  message(SEND_ERROR "Unsupported OS")
endif(WIN32)
# -------------------------------------------------

if (UNIX)
  set (ENABLE_EFFCPP true CACHE  BOOL "Check Effective C++ Guidelines")
endif(UNIX)
# -------------------------------------------------
# find libraries on which this project depends
#
set (ENABLE_EFENCE false CACHE  BOOL "Use Electric Fence memory debugger")

if (ENABLE_EFENCE)
  find_package(efence)
  if (NOT EFENCE_FOUND)
    message(FATAL_ERROR "Could not find Electric Fence memory debugger")
  endif (NOT EFENCE_FOUND)
endif (ENABLE_EFENCE)

# ------------------------------------------------- 
 
find_package(Sqlite)
if (NOT SQLITE_FOUND)
  message(FATAL_ERROR "Could not find SQLite")
endif (NOT SQLITE_FOUND) 

# -------------------------------------------------
# See "Findkutils.cmake" in cmakemodules to figure
# out where it looks and what variables it sets

find_package(kutils)
if(NOT KUTILS_FOUND)
  message(FATAL_ERROR "Could not find kutils")
endif(NOT KUTILS_FOUND)

# -------------------------------------------------
# files to be built into a library go in libsrc/
# any application or testing code goes in src

include_directories(
  ${KUTILS_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/libsrc/
  ${SQLITE_INCLUDE_DIR}
  )
set(KTABMODEL_SRCS
  libsrc/kmodel.cpp
  libsrc/kmodelsql.cpp
  libsrc/emodel.cpp
  libsrc/kstate.cpp
  libsrc/kposition.cpp
  )

add_library(kmodel STATIC ${KTABMODEL_SRCS})

 
# -------------------------------------------------
# Under Linux, default start of DESTINATION path is /usr/local/ 
# so "ktab/lib" denotes destination /usr/local/ktab/lib
# Under Windows, default start of DESTINATION path is "C:\Program Files\kutils\"
# so "ktab/lib" denotes destination C:\Program Files\kutils\ktab\lib

set (KTAB_INSTALL_DIR)
if (WIN32)
  set (KTAB_INSTALL_DIR
  c:/local/ktab
  )
endif(WIN32)
if (UNIX)
  set (KTAB_INSTALL_DIR
  /usr/local/ktab
  )
endif(UNIX)

install(
  TARGETS
    kmodel
  DESTINATION 
    ${KTAB_INSTALL_DIR}/lib) 

install(
  FILES
    libsrc/kmodel.h  
  DESTINATION
    ${KTAB_INSTALL_DIR}/include)  

# -------------------------------------------------
# demo programs

INCLUDE_DIRECTORIES(
  ${KUTILS_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/libsrc/
  ${PROJECT_SOURCE_DIR}/src/
  ${PROJECT_SOURCE_DIR}/csvparser/
  ${EFENCE_INCLUDE_DIR}
  ${SQLITE_INCLUDE_DIR}
  )


# -------------------------------------------------
ADD_EXECUTABLE(leonApp
  src/demoleon.cpp
  )


TARGET_LINK_LIBRARIES(leonApp
  kmodel
  ${KUTILS_LIBRARY}
  ${EFENCE_LIBRARIES}
  ${SQLITE_LIBRARIES}
  )

# -------------------------------------------------
ADD_EXECUTABLE(mtchApp
  src/demomtch.cpp
  )


TARGET_LINK_LIBRARIES(mtchApp
  kmodel
  ${KUTILS_LIBRARY}
  ${EFENCE_LIBRARIES}
  ${SQLITE_LIBRARIES}
  )

# -------------------------------------------------
ADD_EXECUTABLE(demomodel
  src/demo.cpp
  src/edemo.cpp
  src/sqlitedemo.cpp
  src/zactor.cpp
  csvparser/csv_parser.cpp
  )

TARGET_LINK_LIBRARIES(demomodel
  kmodel
  ${KUTILS_LIBRARY}
  ${EFENCE_LIBRARIES}
  ${SQLITE_LIBRARIES}
  )
# -------------------------------------------------
# show some useful status/debugging information
message(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message(STATUS "KUTILS_INCLUDE_DIR: " ${KUTILS_INCLUDE_DIR})
message(STATUS "KTAB_INSTALL_DIR: " ${KTAB_INSTALL_DIR})
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

# --------------------------------------------
# Copyright KAPSARC. Open source MIT License.
# --------------------------------------------
