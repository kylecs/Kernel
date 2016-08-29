#ifndef H_STRING
#define H_STRING
#include <stdint.h>

enum NUMBERS{
	ZERO = '0',
	ONE = '1',
	TWO = '2',
	THREE = '3',
	FOUR = '4',
	FIVE = '5',
	SIX = '6',
	SEVEN = '7',
	EIGHT = '8',
	NINE = '9'

};
uint16_t strlen(char* c);
char digit_to_char(uint8_t input);
char* int_to_string(int32_t input);
char* substring(char* str, uint16_t start, uint16_t end);
char* concat(char* str1, char* str2);
#endif
