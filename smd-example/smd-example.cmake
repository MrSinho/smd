cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SMD_BINARIES_DIR)
message(FATAL_ERROR "smd cmake error: missing SMD_BINARIES_DIR")
endif()

if (NOT DEFINED SMD_ROOT_DIR)
message(FATAL_ERROR "smd cmake error: missing SMD_ROOT_DIR")
endif()



function(build_smd_example)
add_executable(smd-example 
    ${SMD_ROOT_DIR}/smd-example/src/smd-example.c
)
target_link_libraries(smd-example PUBLIC smd)
if (WIN32)
set_target_properties(smd-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${SMD_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${SMD_BINARIES_DIR}/windows
)
else()
set_target_properties(smd-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${SMD_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()