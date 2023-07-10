#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <smd/smd.h>

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	
	//
	//READ EXAMPLE
	//
	SmdFileHandle* p_handle = smdAllocateFileHandle();
	smdError(p_handle == NULL, "failed allocating smd handle memory", return 0);

	uint8_t r = 1;

	//
	//FIRST FILE
	//
	
	smdError(
		smdReadFile("../smd-sample/descriptions/application.smd", p_handle) == 0,
		"application.smd: failed reading source file",
		return -1
	);

	smdError(
		smdParseMemory(p_handle) == 0,
		"application.smd: failed parsing file",
		return -1
	);

	smdError(
		smdDebugPrintFileHandle(p_handle, 1) == 0,
		"application.smd: failed printing handle properties",
		return -1
	);

	
	uint8_t run[2] = { 0 };
	smdError(
		smdAccessVarByIndex(p_handle, 0, NULL, &run) == 0,
		"application.smd: failed reading variable run",
		return -1
	);

	char frame_update[32];//sizeof str256
	smdError(
		smdAccessVarByName(p_handle, "frame_update", NULL, frame_update) == 0,
		"application.smd: failed reading variable frame_update",
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
		smdReadFile("../smd-sample/descriptions/scene.smd", p_handle) == 0,
		"scene.smd: failed reading source file",
		return -1
	);

	smdError(
		smdParseMemory(p_handle) == 0,
		"scene.smd: failed parsing file",
		return -1
	);

	smdError(
		smdDebugPrintFileHandle(p_handle, 1) == 0,
		"scene.smd: failed printing handle properties",
		return -1
	);

	char identity_name[32] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "identity_name", NULL, identity_name) == 0,
		"scene.smd: failed reading variable identity_name",
		return -1
	);

	uint32_t mesh_vertex_count = 0;
	smdError(
		smdAccessVarByName(p_handle, "mesh_vertex_count", NULL, &mesh_vertex_count) == 0,
		"scene.smd: failed reading variable mesh_vertex_count",
		return -1
	);

	float mesh_vertices[6] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "mesh_vertices", NULL, mesh_vertices) == 0,
		"scene.smd: failed reading variable mesh_vertices",
		return -1
	);

	float transform_position[3] = { 0 };
	smdError(
		smdAccessVarByName(p_handle, "transform_position", NULL, transform_position) == 0,
		"scene.smd: failed reading variable transform_position",
		return -1
	);

	float camera_nc = 0.0f;
	smdError(
		smdAccessVarByName(p_handle, "camera_nc", NULL, &camera_nc) == 0,
		"scene.smd: failed reading variable camera_nc",
		return -1
	);

	smdFileHandleRelease(p_handle);

	smdFreeFileHandle(p_handle);


	//
	//WRITE EXAMPLE
	//
	SmdExportHandle* p_export = smdAllocateExportHandle();
	smdError(p_export == NULL, "failed allocating smd export handle memory", return 0);

	smdCommentLine(p_export, "COUNTING FROM 1 TO 3...\n");
	int8_t count[3] = { 1, 2, 3 };
	smdWriteVar(
		p_export, 3, "count", SMD_VAR_TYPE_INT8, count
	);

	smdCommentLine(p_export, "\n");
	float floats[4] = { 2.0f, 4.0f, 6.0f, 8.0f };
	smdWriteVar(
		p_export, 4, "floats", SMD_VAR_TYPE_FLOAT32, floats
	);

	smdCommentLine(p_export, "\n\tSOME GITHUB INFO HERE:\n");
	char see[128] = "@github.com/mrsinho";
	smdWriteVar(
		p_export, 1, "see", SMD_VAR_TYPE_STR1024, see//any str var type is fine
	);

	smdDebugPrintExportHandle(p_export);

	smdWriteFile(p_export, "../smd-sample/descriptions/write.smd");

	smdExportHandleRelease(p_export);

	smdFreeExportHandle(p_export);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus