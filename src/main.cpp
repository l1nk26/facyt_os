#include <output.hpp>
#include <cstdint>
#include <ports.hpp>
#include <cstdint>

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