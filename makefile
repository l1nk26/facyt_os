# Variables
ASM := nasm
CXX := g++
LD := ld
# Opciones de compilación

ASMFLAGS := -f elf32

CXXFLAGS := -Wall -Iinclude -g \
-ffreestanding -fno-exceptions -fno-rtti \
-fno-common -m32 -fno-stack-protector \
-nostartfiles -mno-red-zone -fno-PIE \
-mfpmath=387 -mno-sse -mno-sse2  # esta linea es para evitar mezclar x87fpu con SSE/SSE2 al realizar operaciones de punto flotante

LDFLAGS := -T linker.ld -nostdlib -static -melf_i386

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,object/%.o,$(SRC)) object/_start.o
TARGET := main

# Reglas
all: $(TARGET) iso

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

object/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

object/%.o: src/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf object $(TARGET)
	rm -rf iso my_os.iso

iso: $(TARGET)
	mkdir -p iso/boot/grub
	mv $(TARGET) iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o my_os.iso iso/

run: my_os.iso
	qemu-system-i386 -cdrom my_os.iso

run-debug: my_os.iso
	qemu-system-i386 -cdrom my_os.iso -s -S

.PHONY: all clean iso
