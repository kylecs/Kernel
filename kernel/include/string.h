#pragma once
#include "types.h"

uint16_t strlen(char* c);
char digit_to_char(uint8_t input);
char* substring(char* str, uint16_t start, uint16_t end);
char* concat(char* str1, char* str2);
uint8_t string_starts_with(char* full, char* start);
char** string_split(char* str, char delim, int* arr_length);
uint8_t strcmp(char* str1, char* str2);
int32_t string_index_of_char(char* str, char look);
uint8_t hexchar_to_decimal(char input);
char* number_to_string_template(uint32_t input, uint8_t base, uint8_t sign_bit,
		char* prefix);
//common aliases for number_to_string_template
char* int_to_string(int32_t input);
char* uint_to_string(uint32_t input);
char* int_to_hexstring(int32_t input);
char* uint_to_hexstring(uint32_t input);
char* int_to_binstring(int32_t input);
char* uint_to_binstring(uint32_t input);
