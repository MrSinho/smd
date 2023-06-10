cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


#||||       ||||
#||||PROJECT||||
#||||       ||||
project(shtemplate C)



#||||       ||||
#||||LIBRARY||||
#||||       ||||
function(build_shtemplate)
add_library(shtemplate shtemplate/src/shtemplate.c)
target_include_directories(shtemplate PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shtemplate/include
)
set_target_properties(shtemplate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



#||||       ||||
#||||SAMPLES||||
#||||       ||||
function(build_shtemplate_sample)
add_executable(shtemplate-sample sample/src/shtemplate-sample.c)
target_link_libraries(shtemplate-sample PUBLIC shtemplate)
set_target_properties(shtemplate-sample PROPERTIES
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
option(SH_TEMPLATE_BUILD_SAMPLE CACHE OFF)



#||||    ||||
#||||MAIN||||
#||||    ||||
function(main)
build_shtemplate()
if (SH_TEMPLATE_BUILD_SAMPLE)
build_shtemplate_sample()
endif(SH_TEMPLATE_BUILD_SAMPLE)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()