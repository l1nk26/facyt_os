#pragma once

#define VGA_BUFFER 0xB8000
#define SCREEN_WIDTH 160
#define GRAY 0x07

void putchar(const char c, const char color = GRAY);

void print_int(int num); 

void print_float(float num, int precision = 6); 

void print_string(const char* s); 

void printf(const char* s, ...); 