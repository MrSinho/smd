#ifndef SMD_NATIVE_H
#define SMD_NATIVE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



#define SMD_VAR_NAME_MAX_SIZE       128 
#define SMD_MAX_STACK_VAR_COUNT     2048

#define smdError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("smd error: %s\n", error_msg);\
		failure_expression;\
	}


#define SMD_VAR_INT8_TYPE_BLOCK     "_INT8"//to avoid duplicates with int8_t when comparing string memory
#define SMD_VAR_UINT8_TYPE_BLOCK    "UINT8"
#define SMD_VAR_INT16_TYPE_BLOCK 	"_INT16"
#define SMD_VAR_UINT16_TYPE_BLOCK   "UINT16"
#define SMD_VAR_INT32_TYPE_BLOCK 	"_INT32"
#define SMD_VAR_UINT32_TYPE_BLOCK   "UINT32"
#define SMD_VAR_INT64_TYPE_BLOCK    "_INT64"
#define SMD_VAR_UINT64_TYPE_BLOCK   "UINT64"
#define SMD_VAR_FLOAT32_TYPE_BLOCK  "FLOAT32"
#define SMD_VAR_DOUBLE64_TYPE_BLOCK "DOUBLE64"
#define SMD_VAR_STR128_TYPE_BLOCK   "STR128"
#define SMD_VAR_STR256_TYPE_BLOCK   "STR256"
#define SMD_VAR_STR512_TYPE_BLOCK   "STR512"
#define SMD_VAR_STR1024_TYPE_BLOCK  "STR1024"


#define SMD_VAR_INT8_TYPE_BLOCK_LENGTH     (uint32_t)strlen(SMD_VAR_INT8_TYPE_BLOCK    )
#define SMD_VAR_INT16_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT16_TYPE_BLOCK   )
#define SMD_VAR_INT32_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT32_TYPE_BLOCK   )
#define SMD_VAR_INT64_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_INT64_TYPE_BLOCK   )
#define SMD_VAR_UINT8_TYPE_BLOCK_LENGTH    (uint32_t)strlen(SMD_VAR_UINT8_TYPE_BLOCK   )
#define SMD_VAR_UINT16_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT16_TYPE_BLOCK  )
#define SMD_VAR_UINT32_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT32_TYPE_BLOCK  )
#define SMD_VAR_UINT64_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_UINT64_TYPE_BLOCK  )
#define SMD_VAR_FLOAT32_TYPE_BLOCK_LENGTH  (uint32_t)strlen(SMD_VAR_FLOAT32_TYPE_BLOCK )
#define SMD_VAR_DOUBLE64_TYPE_BLOCK_LENGTH (uint32_t)strlen(SMD_VAR_DOUBLE64_TYPE_BLOCK)
#define SMD_VAR_STR128_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR128_TYPE_BLOCK  )
#define SMD_VAR_STR256_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR256_TYPE_BLOCK  )
#define SMD_VAR_STR512_TYPE_BLOCK_LENGTH   (uint32_t)strlen(SMD_VAR_STR512_TYPE_BLOCK  )
#define SMD_VAR_STR1024_TYPE_BLOCK_LENGTH  (uint32_t)strlen(SMD_VAR_STR1024_TYPE_BLOCK )



typedef enum SmdVarType {
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
	SMD_VAR_TYPE_STR1024       = 13,
	SMD_VAR_TYPE_MAX_ENUM
} SmdVarType;




#define SMD_VAR_LENGTH_BLOCK             "^^ "
#define SMD_VAR_DECLARATION_BLOCK        "!! "
#define SMD_VAR_VALUE_DEFINITION_BLOCK   "--> "
#define SMD_VAR_END_BLOCK                "***"

#define SMD_VAR_LENGTH_BLOCK_LENGTH           (uint32_t)strlen(SMD_VAR_LENGTH_BLOCK)
#define SMD_VAR_DECLARATION_BLOCK_LENGTH      (uint32_t)strlen(SMD_VAR_DECLARATION_BLOCK)
#define SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH (uint32_t)strlen(SMD_VAR_VALUE_DEFINITION_BLOCK)
#define SMD_VAR_END_BLOCK_LENGTH              (uint32_t)strlen(SMD_VAR_END_BLOCK)



typedef char SmdVarName[SMD_VAR_NAME_MAX_SIZE];

typedef struct SmdFileHandle {
	uint32_t   description_src_size;
	char*      description_src;

	uint32_t   var_count;
	uint32_t   vars_ranges  [SMD_MAX_STACK_VAR_COUNT];
	uint32_t   vars_lengths [SMD_MAX_STACK_VAR_COUNT];
	SmdVarName vars_names   [SMD_MAX_STACK_VAR_COUNT];
	SmdVarType vars_types   [SMD_MAX_STACK_VAR_COUNT];

	uint32_t   linear_memory_size;
	void*      p_linear_memory;

} SmdFileHandle;

#define smdAllocateFileHandle() ((SmdFileHandle*)calloc(1, sizeof(SmdFileHandle)))

#define smdFreeFileHandle(p_handle) free(p_handle)



extern uint8_t smdReadFile(
	const char*    src_path,
	SmdFileHandle*  p_handle
);

#define SMD_CHECK_VAR_TYPE(\
		p_first_char,\
		char_idx,\
		var_idx,\
		write_memory,\
		write_var_count,\
		p_handle,\
		TYPE_UPPERCASE,\
		minimum_linear_memory_size\
	)\
	if (\
		((p_handle)->description_src_size - (char_idx)) > SMD_VAR_## TYPE_UPPERCASE ##_TYPE_BLOCK_LENGTH &&\
		memcmp((p_first_char), SMD_VAR_## TYPE_UPPERCASE ##_TYPE_BLOCK, SMD_VAR_## TYPE_UPPERCASE ##_TYPE_BLOCK_LENGTH) == 0\
		) {\
		if (write_memory) {/*this is executed later, because memory has to be allocated first*/\
			(write_var_count)++;\
		}\
		else {\
			(p_handle)->vars_ranges    [(var_idx)]  = (uint32_t)minimum_linear_memory_size/*will be corrected*/;\
			(p_handle)->vars_types[(var_idx)]  = SMD_VAR_TYPE_## TYPE_UPPERCASE;\
			(p_handle)->var_count++;\
			(var_idx)++;\
		}\
	}

#define SMD_CHECK_INT_VAR_VALUE(\
	p_value_char,\
	p_last_char,\
	write_var_count,\
	write_memory_offset,\
	array_idx,\
	p_handle,\
	TYPE_UPPERCASE,\
	type\
)\
	if ((p_handle)->vars_types[(write_var_count) - 1] == SMD_VAR_TYPE_## TYPE_UPPERCASE) {\
		type value = (type)strtol(p_value_char, &p_last_char, 10);\
		int linear_memory_total_offset =\
				(write_memory_offset) - (p_handle)->vars_ranges[(write_var_count) - 1] +\
					((p_handle)->vars_ranges[(write_var_count) - 1] /\
					(p_handle)->vars_lengths[(write_var_count) - 1] * (array_idx));\
		memcpy(\
			&((char*)(p_handle)->p_linear_memory)[linear_memory_total_offset],\
			&value,\
			sizeof(type)\
		);\
		(array_idx)++;\
	}//var size has already been calculated

#define SMD_CHECK_FLT_VAR_VALUE(\
	p_value_char,\
	p_last_char,\
	write_var_count,\
	write_memory_offset,\
	array_idx,\
	p_handle,\
	TYPE_UPPERCASE,\
	type\
)\
	if ((p_handle)->vars_types[(write_var_count) - 1] == SMD_VAR_TYPE_## TYPE_UPPERCASE) {\
		type value = (type)strtof(p_value_char, &(p_last_char));\
		int linear_memory_total_offset =\
				(write_memory_offset) - (p_handle)->vars_ranges[(write_var_count) - 1] +\
					((p_handle)->vars_ranges[(write_var_count) - 1] /\
					(p_handle)->vars_lengths[(write_var_count) - 1] * (array_idx));\
		memcpy(\
			&((char*)(p_handle)->p_linear_memory)[linear_memory_total_offset],\
			&value,\
			sizeof(type)\
		);\
		(array_idx)++;\
	}//var size has already been calculated

#define SMD_CHECK_STR_VAR_VALUE(\
		p_first_char,\
		char_idx,\
		name_char_idx,\
		write_var_count,\
		write_memory_offset,\
		p_handle,\
		TYPE_UPPERCASE\
	)\
	if ((p_handle)->vars_types[(write_var_count) - 1] == SMD_VAR_TYPE_## TYPE_UPPERCASE) {\
		uint32_t string_length = (name_char_idx) - (char_idx) - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;\
		if (string_length > (p_handle)->vars_ranges[(write_var_count) - 1]) {\
			/*error*/\
		}\
		memcpy(\
			&((char*)(p_handle)->p_linear_memory)[(write_memory_offset) - (p_handle)->vars_ranges[(write_var_count) - 1]],\
			(p_first_char) + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,\
			string_length\
		);\
		((char*)(p_handle)->p_linear_memory)[(write_memory_offset) - (p_handle)->vars_ranges[(write_var_count) - 1] + string_length] = '\0';\
		break;\
	}//var size has already been calculated

extern uint8_t smdParseMemory(
	SmdFileHandle* p_handle
);

extern uint8_t smdDebugPrintFileHandle(
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

extern uint8_t smdFileHandleRelease(
	SmdFileHandle * p_handle
);



#define SMD_EXPORT_MAX_LINE_LENGTH 2048
#define SMD_EXPORT_MAX_LINE_COUNT  (SMD_MAX_STACK_VAR_COUNT * 2)

typedef char SmdLine[SMD_EXPORT_MAX_LINE_LENGTH];

typedef struct SmdExportHandle {
	
	uint32_t   line_count;
	SmdLine    smd_lines   [SMD_EXPORT_MAX_LINE_COUNT];

	uint32_t   vars_ranges [SMD_MAX_STACK_VAR_COUNT];
	uint32_t   vars_lengths[SMD_MAX_STACK_VAR_COUNT];
	SmdVarName vars_names  [SMD_MAX_STACK_VAR_COUNT];
	SmdVarType vars_types  [SMD_MAX_STACK_VAR_COUNT];
	uint32_t   vars_indices[SMD_MAX_STACK_VAR_COUNT];

} SmdExportHandle;

#define smdAllocateExportHandle() ((SmdExportHandle*)calloc(1, sizeof(SmdExportHandle)))

#define smdFreeExportHandle(p_handle) free(p_handle)


#define SMD_WRITE_LINE_START(p_handle, s_type, length, name)\
	snprintf((p_handle)->smd_lines[(p_handle)->line_count], SMD_EXPORT_MAX_LINE_LENGTH,\
		"%s    %s%i    %s%s    %s",\
		s_type,\
		SMD_VAR_LENGTH_BLOCK, length,\
		SMD_VAR_DECLARATION_BLOCK, name,\
		SMD_VAR_VALUE_DEFINITION_BLOCK\
	);\

#define SMD_WRITE_LINE_END(p_handle)\
	strcat((p_handle)->smd_lines[(p_handle)->line_count], "    ");\
	strcat((p_handle)->smd_lines[(p_handle)->line_count], SMD_VAR_END_BLOCK);\
	strcat((p_handle)->smd_lines[(p_handle)->line_count], "\n");

// i or u
#define SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, type, t_size)\
	SMD_WRITE_LINE_START(p_handle, s_type, length, name);\
	\
	char s_value[SMD_MAX_STACK_VAR_COUNT] = { 0 };\
	\
	for (uint32_t var_value_idx = 0; var_value_idx < length; var_value_idx++) {\
		type var_value = ((type*)p_var_values)[var_value_idx];\
		\
		snprintf(s_value, SMD_EXPORT_MAX_LINE_LENGTH,\
			"%" PRI ## t_size " ",\
			var_value\
		);\
		\
		strcat((p_handle)->smd_lines[(p_handle)->line_count], s_value);\
		memset(s_value, 0, SMD_MAX_STACK_VAR_COUNT);\
	}\
	\
	SMD_WRITE_LINE_END(p_handle)

#define SMD_WRITE_FLT_VAR_VALUES(p_handle, s_type, length, name, type)\
	SMD_WRITE_LINE_START(p_handle, s_type, length, name);\
	\
	char s_value[SMD_MAX_STACK_VAR_COUNT] = { 0 };\
	\
	for (uint32_t var_value_idx = 0; var_value_idx < length; var_value_idx++) {\
		type var_value = ((type*)p_var_values)[var_value_idx];\
		\
		snprintf(s_value, SMD_EXPORT_MAX_LINE_LENGTH,\
			"%f ",\
			var_value\
		);\
		\
		strcat((p_handle)->smd_lines[(p_handle)->line_count], s_value);\
		memset(s_value, 0, SMD_MAX_STACK_VAR_COUNT);\
	}\
	\
	SMD_WRITE_LINE_END(p_handle)

#define SMD_WRITE_STR_VAR_VALUES(p_handle, s_type, length, name, p_string)\
	SMD_WRITE_LINE_START(p_handle, s_type, length, name);\
	strcat((p_handle)->smd_lines[(p_handle)->line_count], (char*)(p_string));\
	SMD_WRITE_LINE_END(p_handle)

extern uint8_t smdWriteLine(
	SmdExportHandle* p_handle,
	uint32_t         length,
	char*            name,
	SmdVarType       var_type,
	void*            p_var_values
);

extern uint8_t smdCommentLine(
	SmdExportHandle* p_handle,
	SmdLine          line
);

extern uint8_t smdDebugPrintExportHandle(
	SmdExportHandle* p_handle
);

extern uint8_t smdWriteFile(
	SmdExportHandle* p_handle,
	char*            dst_path
);

extern uint8_t smdExportHandleRelease(
	SmdExportHandle* p_handle
);


#ifdef __cplusplus
}
#endif//_cplusplus

#endif//SMD_NATIVE_H