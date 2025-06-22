unsigned char inb(unsigned short const port) {
    #if defined(__GNUC__) || defined(__clang__)
        unsigned char value;
        asm volatile (
            "inb %1, %0"
            : "=a" (value)
            : "Nd" (port)
        );
        return value;
    #endif
    return 0; // Fallback return value if inline assembly is not supported
}

void outb(unsigned short const port, unsigned char value) {
    #if defined(__GNUC__) || defined(__clang__)
        asm volatile (
            "outb %0, %1"
            :
            : "a" (value), "Nd" (port)
        );
    #endif
}