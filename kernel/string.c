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
