#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <smd-native/smd-native.h>

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	
	SmdFileHandle* p_handle = smdAllocateFileHandle();
	smdNativeError(p_handle == NULL, "failed allocating smd handle memory", return 0);

	uint8_t r = 1;

	r = r && smdReadFile("../smd-read-sample/descriptions/application.desc", p_handle);

	smdNativeError(
		p_handle->description_src == NULL, "failed reading file", return -1
	);

	smdParseMemory(p_handle);

	smdDebugPrint(p_handle, 1);

	
	uint8_t run = 0;
	smdAccessVarByIndex(p_handle, 0, NULL, &run);

	char frame_update_function_name[32];
	smdAccessVarByName(p_handle, "frame_update", NULL, frame_update_function_name);


	smdFileHandleRelease(p_handle);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus