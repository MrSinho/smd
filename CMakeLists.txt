cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


#||||       ||||
#||||PROJECT||||
#||||       ||||
project(smd C)



#||||          ||||
#||||SMD NATIVE||||
#||||          ||||
function(build_smd)
add_library(smd smd/src/smd.c)
target_include_directories(smd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/smd/include
)
set_target_properties(smd PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()

#||||              ||||
#||||SMD NATIVE APP||||
#||||              ||||
#function(build_smd_native_app)
#add_executable(smd-app smd/src/smd-app.c)
#target_include_directories(smd-app PUBLIC 
#    ${CMAKE_CURRENT_SOURCE_DIR}/smd/include
#)
#set_target_properties(smd-app PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
#    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
#)
#endfunction()



#||||               ||||
#||||SMD READ SAMPLE||||
#||||               ||||
function(build_smd_read_sample)
add_executable(smd-read-sample smd-read-sample/src/smd-read-sample.c)
target_link_libraries(smd-read-sample PUBLIC smd)
set_target_properties(smd-read-sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()

#||||           ||||
#||||OS SPECIFIC||||
#||||           ||||
if (WIN32)
else()
endif(WIN32)
if(UNIX)
else()
endif(UNIX)

#||||     ||||
#||||FLAGS||||
#||||     ||||
option(SMD_BUILD_SAMPLE CACHE OFF)



#||||    ||||
#||||MAIN||||
#||||    ||||
function(main)
build_smd()
if (SMD_BUILD_SAMPLE)
build_smd_read_sample()
endif(SMD_BUILD_SAMPLE)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()