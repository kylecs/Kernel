#include "include/pci.h"
#include "include/types.h"

char* DEVICE_CLASS_LIST[] = {
	"Unclassified device",
	"Mass storage controller",
	"Network controller",
	"Display controller",
	"Multimedia controller",
	"Memory controller",
	"Bridge",
	"Communication controller",
	"Generic system peripheral",
	"Input device controller",
	"Docking station",
	"Processor",
	"Serial bus controller",
	"Wireless controller",
	"Intelligent controller",
	"Satellite communications controller",
	"Encreption controller",
	"Signal processing controller",
	"Processing accelerators",
	"Non-essential instrumentation", //offset 0x13
};

pci_device_t devices[MAX_PCI_FUNCTIONS];
uint16_t device_counter = 0;
bool scanned = false;

void pci_print_device(pci_device_t device){
	uprintf("BUS: %s, ", device.bus);
	uprintf("DEVICE: %s, ", device.device);
	uprintf("FUNCTION: %s\n", device.function);
	printsf("VENDORID: %s ", uint_to_hexstring(device.vendor_id));
	printsf("DEVICEID: %s\n", uint_to_hexstring(device.device_id));
	printsf("CLASS: %s, ", uint_to_hexstring(device.class));
	printsf("SUBCLASS: %s, ", uint_to_hexstring(device.subclass));
	printsf("HEADER: %s\n", uint_to_hexstring(device.header_type));
	if(device.class > 0x13){
		if(device.class == 0x40){
			println("Coprocessor");
		}else if(device.class == 0xff){
			println("Unassigned class");
		}else{
			println("Unknown");
		}
	}else{
		println(DEVICE_CLASS_LIST[device.class]);
	}
	println("");
}

void pci_print_all(char* args){
	if(!scanned){
		pci_scan();
		scanned = true;
	}
	for(int i = 0; i < device_counter; i++){
		pci_print_device(devices[i]);
	}
}

//BAR = base address registers
uint32_t pci_query(uint32_t address) {

	outdw(PCI_CONFIG, address);
	uint32_t got = indw(PCI_DATA);
	return got;
}

uint32_t pci_make_address(uint8_t bus, uint8_t device, uint8_t function,
													uint8_t reg){
	return 0 | (1 << 31) | (bus << 16) | ((device & 0x1f) << 11) |
			((function & 0x7) << 8) | ((reg & 0xFC));
}

void pci_scan_device(uint8_t bus, uint8_t device){
	uint8_t continue_scan = 1;
	uint8_t function = 0;
	while(continue_scan){
		uint32_t address = pci_make_address(bus, device, function, 0);
		uint32_t result = pci_query(address);
		if(result == 0xFFFFFFFF){
			continue_scan = false;
		}else{
			//valid function, print info
			uint16_t vendor_id = result & 0xFFFF;
			uint16_t device_id = (result >> 16) & 0xFFFF;

			uint32_t reg_data = pci_query(pci_make_address(bus, device, function, 0x08));
			uint8_t class = (reg_data >> 24) & 0xFF;
			uint8_t subclass = (reg_data >> 16) & 0xFF;

			uint32_t header_blob =
				pci_query(pci_make_address(bus, device, function, 0x0C));
			uint8_t header_type = (header_blob >> 16) & 0xFF;

			pci_device_t dev;
			dev.address = address;
			dev.bus = bus;
			dev.device = device;
			dev.function = function;
			dev.vendor_id = vendor_id;
			dev.device_id = device_id;
			dev.class = class;
			dev.subclass = subclass;
			dev.header_type = header_type;
			devices[device_counter] = dev;
			device_counter++;


			function++;
		}
	}
}

void pci_scan_bus(uint8_t bus){
	uint8_t continue_scan = 1;
	uint8_t device = 0;
	while(continue_scan){
		uint32_t result = pci_query(pci_make_address(bus, device, 0, 0));
		if(result == 0xFFFFFFFF){
			continue_scan = 0;
		}else{
			//valid device, scan it
			pci_scan_device(bus, device);
			device++;
		}
	}
}

void pci_scan(){
	print("Scanning for PCI devices ... ");
	uint8_t continue_scan = 1;
	uint8_t bus = 0;
	while(continue_scan){
		uint32_t result = pci_query(pci_make_address(bus, 0, 0, 0));
		if(result == 0xFFFFFFFF){
			continue_scan = 0;
		}else{
			//valid bus, scan it
			pci_scan_bus(bus);
			bus++;
		}
	}
	println("done");
}
