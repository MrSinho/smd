#ifndef SMD_NATIVE_H
#define SMD_NATIVE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



#define SMD_VAR_NAME_MAX_SIZE       64
#define SMD_VAR_MAX_HIERARCY_LENGTH 64
#define SMD_MAX_STACK_VAR_COUNT     2048

#define smdNativeError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("smd native error: %s\n", error_msg);\
		failure_expression;\
	}


#define SMD_VAR_INT8_TYPE_BLOCK     "INT8"
#define SMD_VAR_UINT8_TYPE_BLOCK    "UINT8"
#define SMD_VAR_INT16_TYPE_BLOCK 	"INT16"
#define SMD_VAR_UINT16_TYPE_BLOCK   "UINT16"
#define SMD_VAR_INT32_TYPE_BLOCK 	"INT32"
#define SMD_VAR_UINT32_TYPE_BLOCK   "UINT32"
#define SMD_VAR_INT64_TYPE_BLOCK    "INT64"
#define SMD_VAR_UINT64_TYPE_BLOCK   "UINT64"
#define SMD_VAR_STR128_TYPE_BLOCK   "STR128"
#define SMD_VAR_STR256_TYPE_BLOCK   "STR256"
#define SMD_VAR_STR512_TYPE_BLOCK   "STR512"
#define SMD_VAR_STR1024_TYPE_BLOCK  "STR1024"


#define SMD_VAR_INT8_TYPE_BLOCK_LENGTH     (uint32_t)strlen(SMD_VAR_INT8_TYPE_BLOCK  )
#define SMD_VAR_INT16_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT16_TYPE_BLOCK )
#define SMD_VAR_INT32_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT32_TYPE_BLOCK )
#define SMD_VAR_INT64_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT64_TYPE_BLOCK )
#define SMD_VAR_UINT8_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_UINT8_TYPE_BLOCK )
#define SMD_VAR_UINT16_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT16_TYPE_BLOCK)
#define SMD_VAR_UINT32_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT32_TYPE_BLOCK)
#define SMD_VAR_UINT64_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT64_TYPE_BLOCK)
#define SMD_VAR_STR128_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR128_TYPE_BLOCK  )
#define SMD_VAR_STR256_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR256_TYPE_BLOCK  )
#define SMD_VAR_STR512_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR512_TYPE_BLOCK  )
#define SMD_VAR_STR1024_TYPE_BLOCK_LENGTH  (uint32_t)strlen(SMD_VAR_STR1024_TYPE_BLOCK )



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




#define SMD_VAR_DECLARATION_BLOCK        "!! "
#define SMD_VAR_VALUE_DEFINITION_BLOCK   "--> "

#define SMD_VAR_DECLARATION_BLOCK_LENGTH      (uint32_t)strlen(SMD_VAR_DECLARATION_BLOCK)
#define SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH (uint32_t)strlen(SMD_VAR_VALUE_DEFINITION_BLOCK)



typedef char SmdVarName[SMD_VAR_NAME_MAX_SIZE];

typedef struct SmdFileHandle {
	uint32_t         description_src_size;
	char*            description_src;

	uint32_t         var_count;
	uint32_t         vars_ranges     [SMD_MAX_STACK_VAR_COUNT];
	SmdVarName       vars_names      [SMD_MAX_STACK_VAR_COUNT];
	SmdVarTypeFlags  vars_type_flags [SMD_MAX_STACK_VAR_COUNT];

	uint32_t         linear_memory_size;
	void*            p_linear_memory;

} SmdFileHandle;

#define smdAllocateFileHandle() ((SmdFileHandle*)calloc(1, sizeof(SmdFileHandle)))


extern uint8_t smdReadFile(
	const char*    src_path,
	SmdFileHandle*  p_handle
);

extern uint8_t smdParseMemory(
	SmdFileHandle* p_handle
);

extern uint8_t smdDebugPrint(
	SmdFileHandle* p_handle,
	uint8_t        print_all
);

extern uint8_t smdAccessVarByRegion(
	SmdFileHandle* p_handle,
	uint32_t       offset,
	uint32_t       size,
	void*          p_dst
);

extern uint8_t smdAccessVarByIndex(
	SmdFileHandle* p_handle,
	uint32_t       idx,
	uint32_t*      p_size,
	void*          p_dst
);

extern uint8_t smdAccessVarByName(
	SmdFileHandle* p_handle,
	const char*    var_name,
	uint32_t*      p_size,
	void*          p_dst
);

extern uint8_t smdNativeExport(
	SmdFileHandle* p_handle,
	const char*    dst_path
);

extern uint8_t smdFileHandleRelease(
	SmdFileHandle* p_handle
);


#ifdef __cplusplus
}
#endif//_cplusplus

#endif//SMD_NATIVE_H