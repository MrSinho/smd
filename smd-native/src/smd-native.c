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
							if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_UINT8) {
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
							if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_INT16) {
								int16_t value = (int16_t)atoi(
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - sizeof(int16_t)],
									&value,
									sizeof(int16_t)
								);//var size already calculated
							}

							//[...]

							//
							//STR128
							//
							if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR128) {
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 128],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 1] = '\0';
							}

							//
							//STR256
							//
							if (p_handle->vars_type_flags[write_var_idx - 1] == SMD_VAR_TYPE_STR256) {
								memcpy(
									&((char*)p_handle->p_linear_memory)[write_memory_offset - 256],
									p_first_char + SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH,
									name_char_idx - char_idx - SMD_VAR_VALUE_DEFINITION_BLOCK_LENGTH
								);//var size already calculated
								((char*)p_handle->p_linear_memory)[write_memory_offset - 1] = '\0';
								//char* string = &((char*)p_handle->p_linear_memory)[write_memory_offset - 256];
							}


							break;
						}
					}
				}
			}
		}

		if (!write_memory) {
			//p_handle->p_vars_ranges    = calloc(p_handle->var_count, sizeof(uint32_t));
			//p_handle->p_vars_names     = calloc(p_handle->var_count, sizeof(SmdVarName));
			//p_handle->p_vars_names     = calloc(p_handle->var_count, sizeof(SmdVarName));
			//p_handle->p_var_type_flags = calloc(p_handle->var_count, sizeof(SmdVarTypeFlags));
			p_handle->p_linear_memory    = calloc(p_handle->linear_memory_size, 1);
			
			//smdNativeError(p_handle->p_vars_ranges    == NULL, "invalid vars ranges memory",     return 0);
			//smdNativeError(p_handle->p_vars_names     == NULL, "invalid vars names memory",      return 0);
			//smdNativeError(p_handle->p_var_type_flags == NULL, "invalid vars type flags memory", return 0);
			smdNativeError(p_handle->p_linear_memory == NULL, "invalid handle linear memory", return 0);
		}

	}
	

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus