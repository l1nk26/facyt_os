section .multiboot_header
header_start:
      ; Magic number para Multiboot2
    dd 0xE85250D6            ; Magic (Multiboot2)
    dd 0                     ; Arquitectura (0 = x86_32 protegido)
    dd header_end - header_start  ; Longitud del header
      ; Checksum
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

     ; Tags (opcional, pero GRUB espera al menos uno de terminación)
     ; Tag de terminación
    dw 0     ; Type = 0
    dw 0     ; Flags = 0
    dd 8     ; Size = 8
header_end:

section .text
bits 32                     ; GRUB arranca en modo 32-bit
global _start
extern kmain                ; Función en C++

_start:
     ; Configurando pila
    mov esp, stack_top

    ; Enable FPU and disable emulation
    mov eax, cr0
    and eax, ~(1 << 2)  ; Clear CR0.EM (bit 2)
    or  eax, (1 << 1)   ; Set CR0.MP (bit 1)
    mov cr0, eax
    ; Enable FPU and SSE (if needed)
    fninit               ; Initialize FPU (resets registers & control word)
    fwait                ; makes the cpu waits for FPU to finish
    fldz                  ; Cargar 0.0 en el registro FPU
     


     ; Llamando a kmain() en C++
    call kmain

    ; ;Si kmain retorna, lo cual no debería suceder, pero si acaso bucle infinito
    cli ; desactiva las interrupciones
    hlt ; espera a que ocurra una interrupción
    ; Bucle infinito para evitar salir del kernel

section .data
    value1 dd 2.5
    value2 dd 5.4

section .bss
stack_bottom:
    resb 8192  ; Reservar espacio para la pila (8 KB)

     ; Definir el tope de la pila
     ; (debe ser múltiplo de 4 para alineación)
stack_top: