#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <smd/smd.h>

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	
	SmdFileHandle* p_handle = smdAllocateFileHandle();
	smdError(p_handle == NULL, "failed allocating smd handle memory", return 0);

	uint8_t r = 1;

	//
	//FIRST FILE
	//
	
	smdError(
		smdReadFile("../smd-read-sample/descriptions/application.smd", p_handle) == 0,
		"application.smd: failed reading source file",
		return -1
	);

	smdError(
		smdParseMemory(p_handle) == 0,
		"application.smd: failed parsing file",
		return -1
	);

	smdError(
		smdDebugPrint(p_handle, 1) == 0,
		"application.smd: failed printing handle properties",
		return -1
	);

	
	uint8_t run[2] = { 0 };
	smdError(
		smdAccessVarByIndex(p_handle, 0, NULL, &run) == 0,
		"application.smd: failed reading variable memory by index",
		return -1
	);

	char frame_update_function_name[32];//sizeof str256
	smdError(
		smdAccessVarByName(p_handle, "frame_update", NULL, frame_update_function_name) == 0,
		"application.smd: failed accessing variable memory by name",
		return -1
	);

	smdError(
		smdFileHandleRelease(p_handle) == 0,
		"application.smd: failed realeasing src handle",
		return 0
	);

	//
	//SECOND FILE
	//
	smdError(
		smdReadFile("../smd-read-sample/descriptions/scene.smd", p_handle) == 0,
		"scene.smd: failed reading source file",
		return -1
	);

	smdError(
		smdParseMemory(p_handle) == 0,
		"scene.smd: failed parsing file",
		return -1
	);

	smdError(
		smdDebugPrint(p_handle, 1) == 0,
		"scene.smd: failed printing handle properties",
		return -1
	);

	char identity_name[32] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "identity_name", NULL, identity_name) == 0,
		"scene.smd: failed accessing identity_name",
		return -1
	);

	uint32_t mesh_vertex_count = 0;
	smdError(
		smdAccessVarByName(p_handle, "mesh_vertex_count", NULL, &mesh_vertex_count) == 0,
		"scene.smd: failed accessing mesh_vertex_count",
		return -1
	);

	float mesh_vertices[6] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "mesh_vertices", NULL, mesh_vertices) == 0,
		"scene.smd: failed accessing mesh_vertices",
		return -1
	);

	float transform_position[3] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "transform_position", NULL, transform_position) == 0,
		"scene.smd: failed accessing transform_position",
		return -1
	);

	float camera_nc = 0.0f;
	smdError(
		smdAccessVarByName(p_handle, "camera_nc", NULL, &camera_nc) == 0,
		"scene.smd: failed accessing camera_nc",
		return -1
	);

	smdFileHandleRelease(p_handle);

	smdFreeFileHandle(p_handle);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus