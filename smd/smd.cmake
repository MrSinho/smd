cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SMD_BINARIES_DIR)
message(FATAL_ERROR "smd cmake error: missing SMD_BINARIES_DIR")
endif()

if (NOT DEFINED SMD_ROOT_DIR)
message(FATAL_ERROR "smd cmake error: missing SMD_ROOT_DIR")
endif()



function(build_smd)
add_library(smd 
    ${SMD_ROOT_DIR}/smd/src/smd.c
)
target_include_directories(smd PUBLIC 
    ${SMD_ROOT_DIR}/smd/include
)
if (WIN32)
set_target_properties(smd PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${SMD_BINARIES_DIR}/windows
)
else()
set_target_properties(smd PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${SMD_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()