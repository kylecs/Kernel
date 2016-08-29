#ifndef H_MEMORY
#define H_MEMORY
#include <stdint.h>
#include <stddef.h>


void memcpy(void* src, void* dest, size_t size);
void* kalloc(size_t size);

#endif