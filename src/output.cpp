#include <output.hpp>
#include <stdarg.h>
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

void putchar(const char c, const char color) {
    volatile static char* video_memory = (char*)VGA_BUFFER;
    static int position = 0;
    if (c == '\n') {
        position = (position / SCREEN_WIDTH + 1)  * SCREEN_WIDTH;
    } else {
        video_memory[position*2] = c;          // Character
        video_memory[position*2 + 1] = color;        // Attribute byte (light gray on black)
        position++;
    }
}

void print_int(int num) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }
    if (num == 0) {
        putchar('0');
        return;
    }
    char buffer[20];
    int i = 0;
    while (num > 0 && i < 20) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

void print_float(float num, int precision) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }

    int integer_part = num; 
    float fractional_part = num - integer_part;
    
    print_int(integer_part);
    putchar('.');
    
    // Print fractional part
    for (int i = 0; i < precision; i++) { // Print 6 decimal places
        fractional_part *= 10;
        int digit = fractional_part;
        putchar(digit + '0');
        fractional_part -= digit;
    }
}

void print_string(const char* s) {
    if (s != nullptr) {
        while (*s != '\0') {
            putchar(*s);
            s++;
        }
    } else {
        putchar('?');
    }
}


void printf(const char* s, ...) {
    if (s == nullptr) {
        return; // Handle null pointer
    }
    va_list args;
    va_start(args, s);

    while(*s != '\0') {
        if (*s == '%') {
            s++;
            if (*s == 'd') { // Integer
                int num = va_arg(args, int);
                print_int(num);
            } else if (*s == 'f') { // Double
                double num = va_arg(args, double);
                print_float(num);
            } else if(*s == 's') {
                char *str = va_arg(args, char*);
                print_string(str);
            } else if (*s == '%') { // Literal %
                putchar('%');
            } else {
                // Handle unknown type
                putchar('?');
            }
        } else {
            putchar(*s);
        }
        s++;
    }
    va_end(args);
}