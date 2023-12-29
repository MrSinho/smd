function BUILD {
	mkdir build
	cd    build
	mkdir linux
	cd    linux

	cmake ../../ -DSMD_BUILD_EXAMPLE=ON
	cmake --build .
}

BUILD > .shci/linux/build-output.txt
