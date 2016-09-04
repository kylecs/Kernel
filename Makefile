CC = ~/opt/cross/bin/i686-elf-gcc
AS = ~/opt/cross/bin/i686-elf-as
LNFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc
CCFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS = -c
CTMP = $(wildcard kernel/*.c)
ATMP = $(wildcard kernel/*.s)
CFILE = $(subst kernel/, ,$(CTMP))
AFILE = $(subst kernel/, ,$(ATMP))
CSRC = $(CFILE:.c=)
ASRC = $(AFILE:.s=)

OUT = $(wildcard out/*.o)

all: ccompile acompile link

ccompile:
	for file in $(CSRC) ; do \
		$(CC) $(CCFLAGS) kernel/$$file.c -o out/$$file.o ; \
	done

acompile:
	for file in $(ASRC) ; do \
		$(AS) $(ASFLAGS) kernel/$$file.s -o out/$$file.o ; \
	done

link:
		$(CC) $(LNFLAGS) -o latest.bin $(OUT)

run:
	qemu-system-i386 -kernel latest.bin

bochs:
	bochs -q -f bochsrc.txt

clean:
	rm -f $(OUT)
	rm -f latest.bin

buildiso:
	mkdir -p isodir/boot/grub
	cp latest.bin isodir/boot/kernel.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	sudo grub-mkrescue -o bootable.iso isodir

fullbochs: ccompile acompile link buildiso bochs
