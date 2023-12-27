> .shci/windows/build-output.txt (
    mkdir build
    mkdir build/windows
    cd    build/windows

    cmake ../../ -DSMD_BUILD_EXAMPLE=ON
    cmake --build .
)
