#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <smd-native/smd-native.h>

#include <stdio.h>



int main(void) {
	
	SmdFileHandle handle = { 0 };
	uint8_t r = 1;

	r = r && smdReadFile("../smd-read-sample/descriptions/application.desc", &handle);

	smdNativeError(
		handle.description_src == NULL, "failed reading file", return -1
	);

	printf(handle.description_src);

	smdParseMemory(&handle);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus