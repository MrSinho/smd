> .shci/windows/build-output.txt (
	mkdir build
	cd    build
	mkdir windows
	cd    windows

    cmake ../../ -DSMD_BUILD_EXAMPLE=ON
    cmake --build .
)
