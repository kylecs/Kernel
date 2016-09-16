#pragma once
#include "types.h"
//QEMU FLAGS:
//0000001001001111

//VIRTUALBOX FLAGS:
//0001101001100111
typedef struct{
	//data on this struct
	uint32_t flags;//0

	//memory info
	uint32_t mem_lower;//4
	uint32_t mem_upper;//8

	//info on disk os was loaded from
	uint32_t boot_device;//12
	uint32_t cmdline;//16

	//boot modules
	uint32_t mods_count;//20
	uint32_t mods_addr;//24

	//placeholder for some systeminfo
	uint32_t syms1;//28
	uint32_t syms2;//32
	uint32_t syms3;//36
	uint32_t syms4;//40

	//memory map data
	uint32_t mmap_length;//44
	uint32_t mmap_addr;//48

	//drive data
	uint32_t drives_length;//52
	uint32_t drives_addr;//56

	//some sort of configuration table?
	uint32_t config_table;//60

	//c string of boot loader name
	uint32_t boot_loader_name;//64

	//power managment thing
	uint32_t apm_table;//68

	//vesa info
	uint32_t vbe_control_info;//72
	uint32_t vbe_mode_info;//76
	uint16_t vbe_mode;//80
	uint16_t vbe_interface_seg;//82
	uint16_t vbe_interface_off;//84
	int16_t vge_interface_len;//86

} __attribute__((__packed__)) multiboot_header_t;
