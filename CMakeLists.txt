cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(smd C)



set(SMD_VERSION  "pre-release")
set(SMD_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



set(SMD_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE STRING "Default SMD_BINARIES_DIR")
option(SMD_BUILD_EXAMPLE CACHE OFF)



include(${SMD_ROOT_DIR}/smd/smd.cmake)
include(${SMD_ROOT_DIR}/smd-example/smd-example.cmake)



function(main)
build_smd()
if (SMD_BUILD_EXAMPLE)
build_smd_example()
endif(SMD_BUILD_EXAMPLE)
endfunction()



main()