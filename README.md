# Static Memory Description (smd)

A simple `.smd` file format interpreter and serializer, which is a convenient alternative from other serialization formats such as yaml and json, and - in my opinion - it also feels much more familiar with the C programming language.

## Clone and build

```bash
git clone https://github.com/mrsinho/smd.git
mkdir build
cd build
cmake .. -DSMD_BUILD_SAMPLE=ON
cmake --build .
```

Binaries are located at the `bin` directory.

---

## Example file

```
UINT32   ^^ 1  !! entity_count        --> 2                                     *
STR256   ^^ 1  !! identity_name       --> main                                  *
UINT32   ^^ 1  !! mesh_vertex_count   --> 6                                     *
FLOAT32  ^^ 6  !! mesh_vertices       --> -1.0   1.0   0.0   0.0   1.0   1.0    *
FLOAT32  ^^ 3  !! transform_position  --> 0   0   2.5                           *
FLOAT32  ^^ 1  !! camera_fov          --> 75                                    *
FLOAT32  ^^ 1  !! camera_nc           --> 1E-20                                 *
FLOAT32  ^^ 1  !! camera_fc           --> 150                                   *
FLOAT32  ^^ 1  !! camera_speed        --> 5                                     *
FLOAT32  ^^ 1  !! camera_mouse_speed  --> 3                                     *
STR256   ^^ 1  !! camera_flags        --> CAMERA_SETUP_FREE_FLIGHT              *
```

You can write everything on a single line, but ditributing the instructions on multiple lines is just a lot easier to read.

## Data types

First you specify the variable data type between:

```c
typedef enum SmdVarTypeFlags {
	SMD_VAR_TYPE_NOT_SPECIFIED = 0,
	SMD_VAR_TYPE_INT8          = 1,
	SMD_VAR_TYPE_UINT8         = 2,
	SMD_VAR_TYPE_INT16         = 3,
	SMD_VAR_TYPE_UINT16        = 4,
	SMD_VAR_TYPE_INT32         = 5,
	SMD_VAR_TYPE_UINT32        = 6,
	SMD_VAR_TYPE_INT64         = 7,
	SMD_VAR_TYPE_UINT64        = 8,
	SMD_VAR_TYPE_FLOAT32       = 9,
	SMD_VAR_TYPE_DOUBLE64      = 10,
	SMD_VAR_TYPE_STR128        = 11,
	SMD_VAR_TYPE_STR256        = 12,
	SMD_VAR_TYPE_STR512        = 12,
	SMD_VAR_TYPE_STR1024       = 13
} SmdVarTypeFlags;
```

## Variable length

Then you specify the length of the variable, if it's bigger than 1 you're consequently creating an array of data. To specify the variable length use "`^^ `" (space included) immediately followed by the variable length.

## Variable name

The variable name has to be introduced by the "`!! `" (space included) token and it automatically ends at the first space character the reader encounters.

## Values

They are introduced by the "`--> `" block (space included), and, in the case of arrays, multiple values are separated by as many space characters as you wish. 

## End of line

Before declaring a new variable, end the first description with the `*` character.

---

## Parsing

See the [smd-example.c](smd-sample/src/smd-sample.c) file. To access a specific variable, use the following self-explanatory functions:
 *	```c
	uint8_t smdAccessVarByRegion(
		SmdFileHandle* p_handle,
		uint32_t       offset,
		uint32_t       size,
		void*          p_dst
	);
	``` 
 *	```c
	uint8_t smdAccessVarByIndex(
		SmdFileHandle* p_handle,
		uint32_t       idx,
		uint32_t*      p_size,
		void*          p_dst
	);
	``` 
 *	```c
	uint8_t smdAccessVarByName(
		SmdFileHandle* p_handle,
		const char*    var_name,
		uint32_t*      p_size,
		void*          p_dst
	);
	```
 *	```c
	uint8_t smdWriteLine(
		SmdExportHandle* p_handle,
		uint32_t         length,
		char*            name,
		SmdVarType       var_type,
		void*            p_var_values
	)
	```
 *	```c
	uint8_t smdWriteFile(
		SmdExportHandle* p_handle,
		char*            dst_path
	)
	```

---

## To do
 * Code documentation and cleanup