#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "smd-native/smd-native.h"


#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER


uint8_t smdReadFile(
	const char* src_path,
	SmdFileHandle*  p_handle
) {
	smdNativeError(src_path == NULL, "invalid src path memory",        return 0);
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);
	
	FILE*    stream = fopen(src_path, "r");

	smdNativeError(
		stream == NULL, 
		"invalid src file",
		return 0
	);

	(void)fseek(stream, 0, SEEK_END);
	p_handle->description_src_size = ftell(stream);
	(void)fseek(stream, 0, SEEK_SET);

	smdNativeError(
		p_handle->description_src_size == 0,
		"invalid handle src description size",
		return 0
	);

	p_handle->description_src = (char*)calloc(1, p_handle->description_src_size);

	smdNativeError(
		p_handle->description_src == NULL,
		"invalid handle src description memory", 
		return 0
	);

	(void)fread(p_handle->description_src, 1, p_handle->description_src_size, stream);

	return 1;
}

uint8_t smdParseMemory(
	SmdFileHandle* p_handle
) {
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);
	
	smdNativeError(
		p_handle->description_src_size == 0,
		"invalid src description size",
		return 0;
	);

	smdNativeError(
		p_handle->description_src == NULL,
		"invalid src description memory",
		return 0
	);

	uint32_t write_var_idx       = 0;
	uint32_t write_memory_offset = 0;

	for (uint8_t write_memory = 0; write_memory < 2; write_memory++) {
		for (uint32_t char_idx = 0; char_idx < p_handle->description_src_size; char_idx++) {
			char* p_first_char = &p_handle->description_src[char_idx];

			//
			//NEW VARIABLES
			//
			if (
				(p_handle->description_src_size - char_idx) > SMD_VAR_UINT8_TYPE_BLOCK_LENGTH &&
				memcmp(p_first_char, SMD_VAR_UINT8_TYPE_BLOCK, SMD_VAR_UINT8_TYPE_BLOCK_LENGTH) == 0
				) {
				if (write_memory) {
					p_handle->vars_ranges    [write_var_idx]  = (uint32_t)sizeof(uint8_t);
					p_handle->vars_type_flags[write_var_idx]  = SMD_VAR_TYPE_UINT8;
					write_memory_offset                      += (uint32_t)sizeof(uint8_t);
					write_var_idx++;
				}
				else {
					p_handle->linear_memory_size += (uint32_t)sizeof(uint8_t);
					p_handle->var_count++;
				}
			}
			//[...]
			else if (
				(p_handle->description_src_size - char_idx) > SMD_VAR_STR128_TYPE_BLOCK_LENGTH &&
				memcmp(p_first_char, SMD_VAR_STR128_TYPE_BLOCK, SMD_VAR_STR128_TYPE_BLOCK_LENGTH) == 0
				) {
				if (write_memory) {
					p_handle->vars_ranges[write_var_idx]      = 128;
					p_handle->vars_type_flags[write_var_idx]  = SMD_VAR_TYPE_STR128;
					write_memory_offset                      += 128;
					write_var_idx++;
				}
				else {
					p_handle->linear_memory_size += 128;
					p_handle->var_count++;
				}
			}
			else if (
				(p_handle->description_src_size - char_idx) > SMD_VAR_STR256_TYPE_BLOCK_LENGTH &&
				memcmp(p_first_char, SMD_VAR_STR256_TYPE_BLOCK, SMD_VAR_STR256_TYPE_BLOCK_LENGTH) == 0
				) {
				if (write_memory) {
					p_handle->vars_ranges[write_var_idx]      = 256;
					p_handle->vars_type_flags[write_var_idx]  = SMD_VAR_TYPE_STR256;
					write_memory_offset                      += 256;
					write_var_idx++;
				}
				else {
					p_handle->linear_memory_size += 256;
					p_handle->var_count++;
				}
			}

			//
			//VARIABLE NAME
			//
			if (write_memory && write_var_idx != 0) {
				if (
					(p_handle->description_src_size - char_idx) > SMD_VAR_DECLARATION_BLOCK_LENGTH &&
					memcmp(p_first_char, SMD_VAR_DECLARATION_BLOCK, SMD_VAR_DECLARATION_BLOCK_LENGTH) == 0
					) {
					for (uint32_t name_char_idx = char_idx + SMD_VAR_DECLARATION_BLOCK_LENGTH; name_char_idx < p_handle->description_src_size; name_char_idx++) {
						if (p_handle->description_src[name_char_idx] == ' ') {//name found
							memcpy(
								p_handle->vars_names[write_var_idx - 1],//variable has already been calculated
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
					for (uint32_t name_char_idx = char_idx + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH; name_char_idx < p_handle->description_src_size; name_char_idx++) {
						if (p_handle->description_src[name_char_idx] == ' ') {//name found

							//
							//INT8
							//
							if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_INT8) {
								int8_t value = (int8_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(int8_t)],
									&value,
									sizeof(int8_t)
								);//var size already calculated
							}

							//
							//UINT8
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_UINT8) {
								uint8_t value = (uint8_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(uint8_t)],
									&value,
									sizeof(uint8_t)
								);//var size already calculated
							}

							//
							//INT16
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_INT16) {
								int16_t value = (int16_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(int16_t)],
									&value,
									sizeof(int16_t)
								);//var size already calculated
							}

							//
							//UINT16
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_UINT16) {
								uint16_t value = (uint16_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(uint16_t)],
									&value,
									sizeof(uint16_t)
								);//var size already calculated
							}

							//
							//INT32
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_INT32) {
								int32_t value = (int32_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(int32_t)],
									&value,
									sizeof(int32_t)
								);//var size already calculated
							}

							//
							//UINT32
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_UINT32) {
								uint32_t value = (uint32_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(uint32_t)],
									&value,
									sizeof(uint32_t)
								);//var size already calculated
							}

							//
							//INT64
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_INT64) {
								int64_t value = (int64_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(int64_t)],
									&value,
									sizeof(int64_t)
								);//var size already calculated
							}

							//
							//UINT64
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_UINT64) {
								uint64_t value = (uint64_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(uint64_t)],
									&value,
									sizeof(uint64_t)
								);//var size already calculated
							}

							//
							//FLOAT32
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_FLOAT32) {
								float value = (float)atof(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(float)],
									&value,
									sizeof(float)
								);//var size already calculated
							}

							//
							//DOUBLE64
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_DOUBLE64) {
								double value = (double)atof(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(double)],
									&value,
									sizeof(double)
								);//var size already calculated
							}

							//
							//STR128
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR128) {
								uint32_t string_length = name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 128],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									string_length
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 128 + string_length] = '\0';
							}

							//
							//STR256
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR256) {
								uint32_t string_length = name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 256],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									string_length
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 256 + string_length] = '\0';
								//char* string = &((char*)p_handle->p_linear_memory)[write_memory_offset - 256];
							}

							//
							//STR512
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR512) {
								uint32_t string_length = name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 512],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									string_length
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 512 + string_length] = '\0';
							}

							//
							//STR1024
							//
							else if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR1024) {
								uint32_t string_length = name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH;
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 1024],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									string_length
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 1024 + string_length] = '\0';
							}

							break;
						}
					}
				}
			}
		}

		if (!write_memory) {
			p_handle->p_linear_memory = calloc(p_handle->linear_memory_size, 1);
			smdNativeError(p_handle->p_linear_memory == NULL, "invalid handle linear memory", return 0);
		}

	}
	

	return 1;
}

uint8_t smdDebugPrint(
	SmdFileHandle* p_handle,
	uint8_t        print_all
) {
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);

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
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);
	smdNativeError(p_dst    == NULL, "invalid dst memory",             return 0);

	smdNativeError(
		offset >= p_handle->linear_memory_size,
		"invalid src offset",
		return 0
	);

	smdNativeError(
		p_handle->p_linear_memory == NULL,
		"invalid src linear memory",
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
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);
	smdNativeError(p_dst    == NULL, "invalid dst memory",             return 0);

	smdNativeError(
		idx >= p_handle->var_count,
		"invalid src var index",
		return 0
	);

	smdNativeError(
		p_handle->p_linear_memory == NULL,
		"invalid src linear memory",
		return 0
	);

	uint32_t offset = 0;
	for (uint32_t i = 0; i < idx; i++) {
		offset += p_handle->vars_ranges[i];
	}

	memcpy(
		p_dst,
		&((char*)p_handle->p_linear_memory)[offset],
		p_handle->vars_ranges[idx]
	);

	if (p_size != NULL) {
		(*p_size) = p_handle->vars_ranges[idx];
	}

	return 1;
}

uint8_t smdAccessVarByName(
	SmdFileHandle* p_handle,
	const char*    var_name,
	uint32_t*      p_size,
	void*          p_dst
) {
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);
	smdNativeError(var_name == NULL, "invalid var name memory",        return 0);
	smdNativeError(p_dst    == NULL, "invalid dst memory",             return 0);

	smdNativeError(
		p_handle->p_linear_memory == NULL,
		"invalid src linear memory",
		return 0
	);

	uint32_t idx = 0;
	for (uint32_t i = 0; i < p_handle->var_count; i++) {
		if (memcmp(p_handle->vars_names[i], var_name, strlen(var_name)) == 0) {
			idx = i;
			break;
		}
	}

	smdAccessVarByIndex(p_handle, idx, p_size, p_dst);

	return 1;
}

uint8_t smdFileHandleRelease(
	SmdFileHandle* p_handle
) {
	smdNativeError(p_handle == NULL, "invalid smd file handle memory", return 0);

	if (p_handle->description_src != NULL) {
		free(p_handle->description_src);
	}
	if (p_handle->p_linear_memory == NULL) {
		free(p_handle->p_linear_memory);
	}

	memset(p_handle, 0, sizeof(SmdFileHandle));

	return 1;
}


#ifdef __cplusplus
}
#endif//__cplusplus