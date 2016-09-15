#include "include/string.h"
#include "include/terminal.h"

uint16_t strlen(char* c){
	char i;
	uint16_t length = 0;
	while((i = c[length]) != 0){
		length++;
	}
	return length;
}


char digit_to_char(uint8_t input){
	switch(input){
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';
		default:
			return '.';
	}
}

uint8_t hexchar_to_decimal(char input){
	switch(input){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 9;
		case 'a':
		case 'A':
			return 10;
		case 'b':
		case 'B':
			return 11;
		case 'c':
		case 'C':
			return 12;
		case 'd':
		case 'D':
			return 13;
		case 'e':
		case 'E':
			return 14;
		case 'f':
		case 'F':
			return 15;
		default:
			return 0;
	}
}

//common aliases for number_to_string_template
char* int_to_string(int32_t input){
	return number_to_string_template(input, 10, 1, "");
}

char* uint_to_string(uint32_t input){
	return number_to_string_template(input, 10, 0, "");
}

char* int_to_hexstring(int32_t input){
	return number_to_string_template(input, 16, 1, "0x");
}

char* uint_to_hexstring(uint32_t input){
	return number_to_string_template(input, 16, 0, "0x");
}

char* int_to_binstring(int32_t input){
	return number_to_string_template(input, 2, 1, "0b");
}

char* uint_to_binstring(uint32_t input){
	return number_to_string_template(input, 2, 0, "0b");
}

char* number_to_string_template(uint32_t input, uint8_t base, uint8_t sign_bit,
		char* prefix)
{
	uint8_t prefix_len = strlen(prefix);
	uint8_t negative = sign_bit && ((int32_t)input < 0);

	//ignore the sign bit and invert all bits
	if(sign_bit){
		if((int32_t)input < 0){
			input = ~input;
			input += 1;
		}
		input &= 0x7FFFFFFF;

	}

	uint8_t num_len = 1;
	uint32_t temp = input;
	while((temp /= base) > 0){
		num_len++;
	}

	uint8_t full_length = num_len + prefix_len;
	if(negative){
		full_length++;
	}

	char* str = (char*)kalloc(full_length + 1);
	str[full_length] = 0;

	uint32_t targetmod = 1;
	for(uint16_t i = 0; i < (num_len - 1); i++){
		targetmod *= base;
	}

	uint16_t start = (negative ? 1 : 0) + prefix_len;
	for(uint8_t i = start; i < num_len + start; i++){
		str[i] = digit_to_char((input - (input % targetmod)) / targetmod);
		input = input % targetmod;
		targetmod /= base;
	}

	uint8_t prefix_start = 0;
	if(negative){
		str[0] = '-';
		prefix_start = 1;
	}

	for(uint8_t i = prefix_start; i < prefix_len + prefix_start; i++){
		str[i] = prefix[i - prefix_start];
	}

	return str;
}

char* substring(char* str, uint16_t start, uint16_t end){

	char* ret = (char* )kalloc((sizeof(char)) * (end - start + 1));
	ret[end-start] = 0;

	for(uint16_t i = 0; i < (end - start); i++){
		ret[i] = str[i + start];
	}

	return ret;
}
char* concat(char* str1, char* str2){
	uint16_t size1 = strlen(str1);
	uint16_t size2 = strlen(str2);
	char* ret = (char*)kalloc((sizeof(char)) * (size1 + size2 + 1));
	ret[size1+size2] = 0;
	for(uint16_t i = 0; i < size1; i++){
		ret[i] = str1[i];
	}
	for(uint16_t i = 0; i < size2; i++){
		ret[i + size1] = str2[i];
	}
	return ret;
}

uint8_t string_starts_with(char* full, char* start) {
	if(strlen(full) < strlen(start)) {
		return 0;
	}
	for(int i = 0; i < strlen(start); i++) {
		if(full[i] != start[i]){
			return 0;
		}
	}
	return 1;
}

char** string_split(char* str, char delim, int* arr_length) {
	int len = strlen(str);
	int num_delimeters = 0;
	//first we count delimeters
	//we won't check the last character because no string would be able to
	//come after it
	for(int i = 0; i < len - 2; i++){
		if(str[i] == delim){
			num_delimeters++;
		}
	}
	char** str_array = (char**)kalloc(sizeof(char*) * (num_delimeters + 1));
	int str_offset = 0;

	int start = 0;
	int end = 0;
	while(end < len){
		while(str[end] != delim && end < len){
			end++;
		}
		//end should hold index of delimeter now
		char* substr = (char*)kalloc((sizeof(char)) * (end - start + 1));
		memcpy(str + start, substr, end - start);
		start = end + 1;
		end++;
		str_array[str_offset] = substr;
		str_offset++;
	}

	//return necessary data now
	*arr_length = str_offset;
	return str_array;
}

uint8_t strcmp(char* str1, char* str2){
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if(len1 != len2){
		return false;
	}
	for(int i = 0; i < len1; i++){
		if(str1[i] != str2[i]){
			return false;
		}
	}
	return true;
}

int32_t string_index_of_char(char* str, char look){
	for(int i = 0; i < strlen(str); i++){
		if(str[i] == look){
			return i;
		}
	}
	return -1;
}
