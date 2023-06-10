cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


#||||       ||||
#||||PROJECT||||
#||||       ||||
project(smd C)



#||||          ||||
#||||SMD NATIVE||||
#||||          ||||
function(build_smd_native)
add_library(smd-native smd-native/src/smd-native.c)
target_include_directories(smd-native PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/smd-native/include
)
set_target_properties(smd-native PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()

#||||              ||||
#||||SMD NATIVE APP||||
#||||              ||||
function(build_smd_native_app)
add_executable(smd-native-app smd-native/src/smd-native-app.c)
target_include_directories(smd-native-app PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/smd-native/include
)
set_target_properties(smd-native-app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



#||||               ||||
#||||SMD READ SAMPLE||||
#||||               ||||
function(build_smd_read_sample)
add_executable(smd-read-sample smd-read-sample/src/smd-read-sample.c)
target_link_libraries(smd-read-sample PUBLIC smd-native)
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
build_smd_native()
build_smd_native_app()
if (SMD_BUILD_SAMPLE)
build_smd_read_sample()
endif(SMD_BUILD_SAMPLE)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()