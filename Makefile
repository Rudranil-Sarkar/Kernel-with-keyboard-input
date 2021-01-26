GPARMS = -m32 --static -nostdlib -fno-rtti -fno-exceptions -fno-builtin -fno-leading-underscore -fno-use-cxa-atexit
ASPARMS = --32
objects = start.o gdt.o port.o interrupthandle.o interrupt.o kernel.o
LDPARAMS = -melf_i386

%.o: %.cpp
	gcc $(GPARMS) -o $@ -c $<

%.o: %.s
	as $(ASPARMS) -o $@ $<

WolfKernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: WolfKernel.bin
	sudo cp $< /boot/WolfKernel.bin

.PHONY: clean
clean:
	rm -rf $(objects) WolfKernel.bin WolfKernel.iso
	
WolfKernel.iso: WolfKernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp WolfKernel.bin iso/boot/WolfKernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/WolfKernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=WolfKernel.iso iso
	rm -rf iso

run: WolfKernel.iso
	VirtualBox --startvm "Wolfkernel" &
