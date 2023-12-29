cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(smd C)



set(SMD_VERSION  "pre-release")
set(SMD_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



option(SMD_BINARIES_DIR  CACHE OFF)
option(SMD_BUILD_EXAMPLE CACHE OFF)



if (NOT SMD_BINARIES_DIR)
set(SMD_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()



message(STATUS "SMD_VERSION       : ${SMD_VERSION}")
message(STATUS "SMD_ROOT_DIR      : ${SMD_ROOT_DIR}")
message(STATUS "SMD_BINARIES_DIR  : ${SMD_BINARIES_DIR}")
message(STATUS "SMD_BUILD_EXAMPLE : ${SMD_BUILD_EXAMPLE}")



include(${SMD_ROOT_DIR}/smd/smd.cmake)
include(${SMD_ROOT_DIR}/smd-example/smd-example.cmake)



function(main)
build_smd()
if (SMD_BUILD_EXAMPLE)
build_smd_example()
endif(SMD_BUILD_EXAMPLE)
endfunction()



main()