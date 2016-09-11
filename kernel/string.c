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
	if(input > 9){
		return 'E';
	}
	if(input == 0){
		return ZERO;
	}else if(input == 1){
		return ONE;
	}else if(input == 2){
		return TWO;
	}else if(input == 3){
		return THREE;
	}else if(input == 4){
		return FOUR;
	}else if(input == 5){
		return FIVE;
	}else if(input == 6){
		return SIX;
	}else if(input == 7){
		return SEVEN;
	}else if(input == 8){
		return EIGHT;
	}else if(input == 9){
		return NINE;
	}else{
		return 'E';
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


	}
}


char* int_to_string(int32_t input){
	bool negative = false;
	if(input < 0) {
		negative = true;
		input *= -1;
	}

	if(input == 0) {
		char* ret = kalloc(sizeof(char));
		(*ret) = '0';
		return ret;
	}

	uint16_t n = 1;
	uint32_t temp = input;
	while((temp /= 10) >=1){
		n++;
	}
	uint16_t size = n;
	if(negative) {
		size++;
	}
	char* ret = (char*) kalloc(size);

	if(input < 10){
		ret[0] = digit_to_char(input);
		return ret;
	}
	uint32_t targetmod = 1;
	for(uint16_t i = 0; i < (n - 1); i++){
		targetmod *= 10;
	}

	uint16_t start = negative ? 1 : 0;

	if(negative) {
		ret[0] = '-';
	}

	for(uint16_t i = start; i < n + start; i++){
		ret[i] = digit_to_char((input - (input % targetmod) )/ targetmod);

		input = input % targetmod;
		targetmod /= 10;
	}

	return ret;
	//return digit_to_char(n + 1);
}

char* substring(char* str, uint16_t start, uint16_t end){

	char* ret = (char* )kalloc(end - start + 1);


	for(uint16_t i = 0; i < (end - start); i++){
		ret[i] = str[i + start];
	}

	return ret;
}
char* concat(char* str1, char* str2){
	uint16_t size1 = strlen(str1);
	uint16_t size2 = strlen(str2);
	char* ret = (char*)kalloc(strlen(str1) + strlen(str2));
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
		char* substr = (char*)kalloc(end - start + 1);
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
