#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "smd/smd.h"


#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER


uint8_t smdReadFile(
	const char*    src_path,
	SmdFileHandle* p_handle
) {
	smdError(src_path == NULL, "smdReadFile: invalid src path memory",        return 0);
	smdError(p_handle == NULL, "smdReadFile: invalid smd file handle memory", return 0);
	
	FILE*    stream = fopen(src_path, "r");

	smdError(
		stream == NULL, 
		"invalid src file",
		return 0
	);

	(void)fseek(stream, 0, SEEK_END);
	p_handle->description_src_size = ftell(stream);
	(void)fseek(stream, 0, SEEK_SET);

	smdError(
		p_handle->description_src_size == 0,
		"smdReadFile: invalid handle src description size",
		return 0
	);

	p_handle->description_src = (char*)calloc(1, p_handle->description_src_size);

	smdError(
		p_handle->description_src == NULL,
		"smdReadFile: invalid handle src description memory", 
		return 0
	);

	(void)fread(p_handle->description_src, 1, p_handle->description_src_size, stream);

	fclose(stream);

	return 1;
}

uint8_t smdParseMemory(
	SmdFileHandle* p_handle
) {
	smdError(p_handle == NULL, "smdParseMemory: invalid smd file handle memory", return 0);
	
	smdError(
		p_handle->description_src_size == 0,
		"smdParseMemory: invalid src description size",
		return 0;
	);

	smdError(
		p_handle->description_src == NULL,
		"smdParseMemory: invalid src description memory",
		return 0
	);

	uint32_t write_var_idx       = 0;
	uint32_t var_idx             = 0;
	uint32_t write_memory_offset = 0;

	for (uint8_t write_memory = 0; write_memory < 2; write_memory++) {
		for (uint32_t char_idx = 0; char_idx < p_handle->description_src_size; char_idx++) {
			char* p_first_char = &p_handle->description_src[char_idx];

			//
			//NEW VARIABLES
			//
			     SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, INT8,     1)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, UINT8,    1)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, INT16,    2)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, UINT16,   2)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, INT32,    4)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, UINT32,   4)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, INT64,    8)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, UINT64,   8)

			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, FLOAT32,  4)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, DOUBLE64, 8)

			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, STR128,  16)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, STR256,  32)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, STR512,  64)
			else SMD_CHECK_VAR_TYPE(p_first_char, char_idx, var_idx, write_memory, write_var_idx, p_handle, STR1024, 128)

			//
			//MEMORY LENGTH
			//
			if (
				(p_handle->description_src_size - char_idx) > SMD_VAR_LENGTH_BLOCK_LENGTH &&
				memcmp(p_first_char, SMD_VAR_LENGTH_BLOCK, SMD_VAR_LENGTH_BLOCK_LENGTH) == 0
				) {
				for (uint32_t length_digit_idx = char_idx + SMD_VAR_LENGTH_BLOCK_LENGTH; length_digit_idx < p_handle->description_src_size; length_digit_idx++) {
					if (p_handle->description_src[length_digit_idx] == ' ' || p_handle->description_src[length_digit_idx] == '\t') {//length found
						uint32_t var_length                                      = atoi(&p_handle->description_src[char_idx + SMD_VAR_LENGTH_BLOCK_LENGTH]);
						uint32_t var_size                                        = 0;//multiply initial value by variable length
						if (write_memory && (write_var_idx - 1) < p_handle->var_count) {
							var_size                                             = p_handle->vars_ranges[write_var_idx - 1]  * var_length;
							(p_handle)->vars_lengths        [write_var_idx - 1]  = var_length;
							(p_handle)->vars_ranges         [write_var_idx - 1]  = var_size;
							write_memory_offset                                 += var_size;
						}
						else if (!write_memory && var_idx != 0) {
							var_size                                             = p_handle->vars_ranges[var_idx - 1] * var_length;
							(p_handle)->linear_memory_size                      += var_size;
						}
						break;
					}
					else {
						/*error*/
					}
					
				}
			}
			

			//
			//VARIABLE NAME
			//
			if (!write_memory && var_idx != 0) {
				if (
					(p_handle->description_src_size - char_idx) > SMD_VAR_DECLARATION_BLOCK_LENGTH &&
					memcmp(p_first_char, SMD_VAR_DECLARATION_BLOCK, SMD_VAR_DECLARATION_BLOCK_LENGTH) == 0
					) {
					for (uint32_t name_char_idx = char_idx + SMD_VAR_DECLARATION_BLOCK_LENGTH; name_char_idx < p_handle->description_src_size; name_char_idx++) {
						if (p_handle->description_src[name_char_idx] == ' ' || p_handle->description_src[name_char_idx] == '\t') {//name found
							memcpy(
								p_handle->vars_names[var_idx - 1],//variable has already been calculated
								p_first_char + SMD_VAR_DECLARATION_BLOCK_LENGTH,
								name_char_idx - char_idx - SMD_VAR_DECLARATION_BLOCK_LENGTH
							);
							break;
						}
					}
				}
			}

			//
			//VALUE
			//
			if (write_memory && write_var_idx != 0) {
				if (
					(p_handle->description_src_size - char_idx) > SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH &&
					memcmp(p_first_char, SMD_VAR_VALUE_DEFINITION_BLOCK, SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH) == 0
					) {
					uint32_t array_idx        = 0;//always zero if variable has length == 1
					uint32_t literal_offset   = SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;//questo
					char     last_char        = '?';

					for (uint32_t name_char_idx = char_idx + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH; name_char_idx < p_handle->description_src_size; name_char_idx++) {
						
						if (
							(p_handle->description_src[name_char_idx] == ' '  && last_char != ' ' ) ||
							(p_handle->description_src[name_char_idx] == ' '  && last_char != '\t') ||
							(p_handle->description_src[name_char_idx] == '\t' && last_char != ' ' ) ||
							(p_handle->description_src[name_char_idx] == '\t' && last_char != '\t')
							) {//name found
							
							char* p_last_char  = &p_handle->description_src[name_char_idx];
							char* p_value_char = (p_first_char) + (literal_offset);
							(literal_offset)   = (uint32_t)(p_last_char - (p_first_char));


								 SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, INT8,     int8_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, UINT8,    uint8_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, INT16,    int16_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, UINT16,   uint16_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, INT32,    int32_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, UINT32,   uint32_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, INT64,    int64_t)
							else SMD_CHECK_INT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, UINT64,   uint64_t)

							else SMD_CHECK_FLT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, FLOAT32,  float)
							else SMD_CHECK_FLT_VAR_VALUE(p_value_char, p_last_char, write_var_idx, write_memory_offset, array_idx, p_handle, DOUBLE64, double)

							else SMD_CHECK_STR_VAR_VALUE(p_first_char, char_idx, name_char_idx, write_var_idx, write_memory_offset, p_handle, STR128 )
							else SMD_CHECK_STR_VAR_VALUE(p_first_char, char_idx, name_char_idx, write_var_idx, write_memory_offset, p_handle, STR256 )
							else SMD_CHECK_STR_VAR_VALUE(p_first_char, char_idx, name_char_idx, write_var_idx, write_memory_offset, p_handle, STR512 )
							else SMD_CHECK_STR_VAR_VALUE(p_first_char, char_idx, name_char_idx, write_var_idx, write_memory_offset, p_handle, STR1024)

							if (array_idx >= p_handle->vars_lengths[write_var_idx - 1]) {
								break;
							}

						}
						else if (p_handle->description_src[name_char_idx] == '*') {
							break;
						}
						last_char = p_handle->description_src[name_char_idx];
					}
				}
			}
		}

		if (!write_memory) {
			p_handle->p_linear_memory = calloc(p_handle->linear_memory_size, 1);
			smdError(p_handle->p_linear_memory == NULL, "smdParseMemory: invalid handle linear memory", return 0);
		}

	}
	

	return 1;
}

uint8_t smdDebugPrintFileHandle(
	SmdFileHandle* p_handle,
	uint8_t        print_all
) {
	smdError(p_handle == NULL, "smdDebugPrintFileHandle: invalid smd file handle memory", return 0);

	char* description_src = "NULL";
	if (p_handle->description_src != NULL) {
		description_src = p_handle->description_src;
	}

	printf("\
--------------------------------------------------\n\
smd debug message:\n\
handle:               %p\n\
description_src_size: %i\n\
description_src:    \n%s\n\
var_count:   %i\n\
memory_size: %i\n\
"
	, (void*)p_handle, p_handle->description_src_size, description_src, p_handle->var_count, p_handle->linear_memory_size);

	if (print_all) {
		for (uint32_t var_idx = 0; var_idx < p_handle->var_count; var_idx++) {
			printf("-->\n\t\
var_name:  %s\n\t\
var_range: %i\n\
<--\n"
			, p_handle->vars_names[var_idx], p_handle->vars_ranges[var_idx]);
		}
	}
	
	printf("--------------------------------------------------\n");

	return 1;
}

uint8_t smdAccessVarByRegion(
	SmdFileHandle* p_handle,
	uint32_t       offset,
	uint32_t       size,
	void*          p_dst
) {
	smdError(p_handle == NULL, "smdAccessVarByRegion: invalid smd file handle memory", return 0);
	smdError(p_dst    == NULL, "smdAccessVarByRegion: invalid dst memory",             return 0);

	smdError(
		offset >= p_handle->linear_memory_size,
		"smdAccessVarByRegion: invalid src offset",
		return 0
	);

	smdError(
		p_handle->p_linear_memory == NULL,
		"smdAccessVarByRegion: invalid src linear memory",
		return 0
	);

	memcpy(p_dst, &((char*)p_handle->p_linear_memory)[offset], size);

	return 1;
}

uint8_t smdAccessVarByIndex(
	SmdFileHandle* p_handle,
	uint32_t       idx,
	uint32_t*      p_size,
	void*          p_dst
) {
	smdError(p_handle == NULL, "smdAccessVarByIndex: invalid smd file handle memory", return 0);
	smdError(p_dst    == NULL, "smdAccessVarByIndex: invalid dst memory",             return 0);

	smdError(
		idx >= p_handle->var_count,
		"smdAccessVarByIndex: invalid src var index",
		return 0
	);

	smdError(
		p_handle->p_linear_memory == NULL,
		"smdAccessVarByIndex: invalid src linear memory",
		return 0
	);

	uint32_t offset = 0;
	for (uint32_t i = 0; i < idx; i++) {
		offset += p_handle->vars_ranges[i];
	}

	if (p_size != NULL) {
		(*p_size) = p_handle->vars_ranges[idx];
	}
	else {
		memcpy(
			p_dst,
			&((char*)p_handle->p_linear_memory)[offset],
			p_handle->vars_ranges[idx]
		);
	}

	return 1;
}

uint8_t smdAccessVarByName(
	SmdFileHandle* p_handle,
	const char*    var_name,
	uint32_t*      p_size,
	void*          p_dst
) {
	smdError(p_handle == NULL, "smdAccessVarByName: invalid smd file handle memory", return 0);
	smdError(var_name == NULL, "smdAccessVarByName: invalid var name memory",        return 0);
	smdError(p_dst    == NULL, "smdAccessVarByName: invalid dst memory",             return 0);

	smdError(
		p_handle->p_linear_memory == NULL,
		"smdAccessVarByName: invalid src linear memory",
		return 0
	);


	uint32_t idx = SMD_MAX_STACK_VAR_COUNT;
	for (uint32_t i = 0; i < p_handle->var_count; i++) {
		
		if (strcmp(p_handle->vars_names[i], var_name) == 0) {
			idx = i;
			break;
		}
	}

	if (idx == SMD_MAX_STACK_VAR_COUNT) {
		return 1;
	}

	smdAccessVarByIndex(p_handle, idx, p_size, p_dst);

	return 1;
}

uint8_t smdFileHandleRelease(
	SmdFileHandle* p_handle
) {
	smdError(p_handle == NULL, "smdFileHandleRelease: invalid smd file handle memory", return 0);

	if (p_handle->description_src != NULL) {
		free(p_handle->description_src);
	}
	if (p_handle->p_linear_memory == NULL) {
		free(p_handle->p_linear_memory);
	}

	memset(p_handle, 0, sizeof(SmdFileHandle));

	return 1;
}

uint8_t smdWriteVar(
	SmdExportHandle* p_handle,
	uint32_t         length,
	char*            name,
	SmdVarType       var_type,
	void*            p_var_values
) {
	smdError(p_handle     == NULL, "smdWriteLine: invalid handle memory",     return 0);
	smdError(length       == 0,    "smdWriteLine: invalid var length",        return 0);
	smdError(name         == NULL, "smdWriteLine: invalid var name memory",   return 0);
	smdError(p_var_values == NULL, "smdWriteLine: invalid var values memory", return 0);
	
	smdError(
		var_type >= SMD_VAR_TYPE_MAX_ENUM || var_type == SMD_VAR_TYPE_NOT_SPECIFIED,
		"smdWriteLine: invalid var type",
		return 0
	);

	smdError(
		strlen(name) == 0,
		"smdWriteLine: invalid var name",
		return 0
	)

	char s_type[32] = { 0 };
	
	     if (var_type == SMD_VAR_TYPE_INT8    )  { strcpy(s_type, SMD_VAR_INT8_TYPE_BLOCK);     SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, int8_t,  i8);  }
	else if (var_type == SMD_VAR_TYPE_INT16   )  { strcpy(s_type, SMD_VAR_INT16_TYPE_BLOCK);    SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, int16_t, i16); }
	else if (var_type == SMD_VAR_TYPE_INT32   )  { strcpy(s_type, SMD_VAR_INT32_TYPE_BLOCK);    SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, int32_t, i32); }
	else if (var_type == SMD_VAR_TYPE_INT64   )  { strcpy(s_type, SMD_VAR_INT64_TYPE_BLOCK);    SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, int64_t, i64); }

	else if (var_type == SMD_VAR_TYPE_UINT8   )  { strcpy(s_type, SMD_VAR_UINT8_TYPE_BLOCK);    SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, uint8_t,  u8);  }
	else if (var_type == SMD_VAR_TYPE_UINT16  )  { strcpy(s_type, SMD_VAR_UINT16_TYPE_BLOCK);   SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, uint16_t, u16); }
	else if (var_type == SMD_VAR_TYPE_UINT32  )  { strcpy(s_type, SMD_VAR_UINT32_TYPE_BLOCK);   SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, uint32_t, u32); }
	else if (var_type == SMD_VAR_TYPE_UINT64  )  { strcpy(s_type, SMD_VAR_UINT64_TYPE_BLOCK);   SMD_WRITE_INT_VAR_VALUES(p_handle, s_type, length, name, uint64_t, u64); }

	else if (var_type == SMD_VAR_TYPE_FLOAT32 )  { strcpy(s_type, SMD_VAR_FLOAT32_TYPE_BLOCK);  SMD_WRITE_FLT_VAR_VALUES(p_handle, s_type, length, name, float);  }
	else if (var_type == SMD_VAR_TYPE_DOUBLE64)  { strcpy(s_type, SMD_VAR_DOUBLE64_TYPE_BLOCK); SMD_WRITE_FLT_VAR_VALUES(p_handle, s_type, length, name, double); }

	else if (var_type == SMD_VAR_TYPE_STR128  )  { strcpy(s_type, SMD_VAR_STR128_TYPE_BLOCK);   SMD_WRITE_STR_VAR_VALUES(p_handle, s_type, length, name, p_var_values); }
	else if (var_type == SMD_VAR_TYPE_STR256  )  { strcpy(s_type, SMD_VAR_STR256_TYPE_BLOCK);   SMD_WRITE_STR_VAR_VALUES(p_handle, s_type, length, name, p_var_values); }
	else if (var_type == SMD_VAR_TYPE_STR512  )  { strcpy(s_type, SMD_VAR_STR512_TYPE_BLOCK);   SMD_WRITE_STR_VAR_VALUES(p_handle, s_type, length, name, p_var_values); }
	else if (var_type == SMD_VAR_TYPE_STR1024 )  { strcpy(s_type, SMD_VAR_STR1024_TYPE_BLOCK);  SMD_WRITE_STR_VAR_VALUES(p_handle, s_type, length, name, p_var_values); }

	p_handle->line_count++;

	return 1;
}

uint8_t smdCommentLine(
	SmdExportHandle* p_handle,
	SmdLine          line
) {
	smdError(p_handle == NULL, "smdCommentLine: invalid handle memory", return 0);

	memcpy(p_handle->smd_lines[p_handle->line_count], line, sizeof(SmdLine));

	p_handle->line_count++;

	return 1;
}

uint8_t smdDebugPrintExportHandle(
	SmdExportHandle* p_handle
) {
	smdError(p_handle == NULL, "smdDebugPrintExportHandle: invalid handle memory", return 0);

	for (uint32_t line_idx = 0; line_idx < p_handle->line_count; line_idx++) {
		printf(p_handle->smd_lines[line_idx]);
	}

	return 1;
}

uint8_t smdWriteFile(
	SmdExportHandle* p_handle,
	char*            dst_path
) {
	smdError(p_handle == NULL, "smdWriteFile: invalid handle memory",   return 0);
	smdError(dst_path == NULL, "smdWriteFile: invalid dst path memory", return 0);

	uint32_t src_size = 0;
	for (uint32_t line_idx = 0; line_idx < p_handle->line_count; line_idx++) {
		src_size += (uint32_t)strlen(p_handle->smd_lines[line_idx]);
	}

	char* src = calloc(1, src_size);
	smdError(
		src == NULL,
		"smdWriteFile: invalid src buffer memory",
		return 0
	);

	uint32_t write_offset = 0;
	for (uint32_t line_idx = 0; line_idx < p_handle->line_count; line_idx++) {
		memcpy(&src[write_offset], p_handle->smd_lines[line_idx], strlen(p_handle->smd_lines[line_idx]));
		write_offset += (uint32_t)strlen(p_handle->smd_lines[line_idx]);
	}

	FILE* stream = fopen(dst_path, "w");

	fwrite(src, 1, src_size, stream);
	fflush(stream);
	fclose(stream);

	free(src);

	return 1;
}

uint8_t smdExportHandleRelease(
	SmdExportHandle* p_handle
) {
	smdError(p_handle == NULL, "invalid smd export handle memory", return 0);

	memset(p_handle, 0, sizeof(SmdExportHandle));

	return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus