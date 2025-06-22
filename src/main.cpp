#include <output.hpp>
#include <cstdint>
#include <ports.hpp>
#include <cstdint>


void set_vga_font_height(uint8_t height) {

    // when I set 0x3D4 then 0x3D5 changes
    // giving me information about the VGA

    // Unlock CRTC registers (usually needed)
    outb(0x3D4, 0x11);
    outb(0x3D5, inb(0x3D5) & 0x7F);  // Clear bit 7

    // Read current Maximum Scan Line register
    outb(0x3D4, 0x09);
    uint8_t scanlines = inb(0x3D5);

    // Update height (bits 0-4)
    scanlines = (scanlines & 0xE0) | ((height - 1) & 0x1F);
    outb(0x3D4, 0x09);
    outb(0x3D5, scanlines);

    // Adjust vertical timing
    outb(0x3D4, 0x12);  // Vertical Display End
    outb(0x3D5, (height * 25) - 1);  // For 25-line text mode
}

extern "C" void kmain() {

    set_vga_font_height(13); // Set font height to 16 pixels

    const char *message = 
        "\n"
        "Hello, World!\n"
        "Bienvenido a facyt os!\n"
        "literalmente esto no hace nada util\n"
        "un sistema operativo de verdad es mucho mas complejo\n"
        "y esta vaina ni detecta el teclado ._.\n"
    ;

    printf(message);
    printf("pero por fin hice un printf que funciona bien :D %%\n");
    printf("mira esos numeros formateados: %d, %f, %d, %f\n", 12345, 3.14159, -67890, 2.71828);

    for(;;);
}