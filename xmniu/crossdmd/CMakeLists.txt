project(crossdmd)

#SET(CMAKE_BUILD_TYPE "Debug") 

set_property(GLOBAL PROPERTY USE_FOLDERS On)
cmake_minimum_required(VERSION 2.8)

set(PLATFORM X86)

if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM X64)
    MESSAGE(STATUS "Detected 64-bit platform")
else()
    MESSAGE(STATUS "Detected 32-bit platform")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions("-std=c++0x")
  add_definitions(-fno-delete-null-pointer-checks)
endif()

if (WIN32)
  set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS} /wd4819")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  
  add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)  
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)  
  add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)

  if(${PLATFORM} STREQUAL "X64")
    add_definitions("-D_WIN64")
    #Enable extended object support for debug compiles on X64 (not required on X86)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
  endif()
elseif(APPLE)
  MESSAGE("APPLE")
elseif(UNIX)
  message("UNIX")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs/)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/robot
)

LINK_DIRECTORIES(
  ${CMAKE_BINARY_DIR}/libs/lib
)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

######################################################################
#file(GLOB sources_localdir *.cpp *.h *.c)
add_executable(crossdmd crossdmd_main.cpp)

#file(GLOB robot_dir robot/main.cpp robot/*.h robot/*.c)
add_executable(crossdmd_robot robot_main.cpp)
target_link_libraries(crossdmd_robot pthread)
