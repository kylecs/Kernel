#pragma once
#include "types.h"


void memcpy(void* src, void* dest, size_t size);
void* kalloc(size_t size);
void memset(void* dst, uint32_t size, uint8_t fill);
