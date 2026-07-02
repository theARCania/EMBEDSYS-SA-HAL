#include "vga.h"

#define VGA_PIXEL 0xc8000000
#define VGA_CHAR  0xc9000000

static void DrawPixel(int x, int y, int colour) {
    volatile short *vgaptr = (volatile short *)(VGA_PIXEL + (x << 1) + (y << 10));
    *vgaptr = colour;
}

static void drawCircle(int xc, int yc, int x, int y) {
    DrawPixel(xc + x, yc + y, 0x1100); DrawPixel(xc - x, yc + y, 0x1100);
    DrawPixel(xc + x, yc - y, 0x1100); DrawPixel(xc - x, yc - y, 0x1100);
    DrawPixel(xc + y, yc + x, 0x1100); DrawPixel(xc - y, yc + x, 0x1100);
    DrawPixel(xc + y, yc - x, 0x1100); DrawPixel(xc - y, yc - x, 0x1100);
}

static void circleBres(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x) {
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } 
        else d = d + 4 * x + 6;
        x++;
        drawCircle(xc, yc, x, y);
    }
}

static void clear_screen() {
    for (int x = 0; x < 320; x++)
        for (int y = 0; y < 240; y++)
            DrawPixel(x, y, 0);
}

void VGA_DrawRadar() {
    clear_screen();
    for (int y = 0; y < 240; y++) DrawPixel(160, y, 0x1100);
    for (int x = 0; x < 320; x++) DrawPixel(x, 120, 0x1100);
    circleBres(160, 120, 120);
    circleBres(160, 120, 120 - 120 / 3);
    circleBres(160, 120, 120 / 3);
    DrawPixel(160, 120, 0x07e0);
}

static void write_char(int x, int y, char c) {
    volatile char *char_ptr = (volatile char *)(VGA_CHAR | (y << 7) | x);
    *char_ptr = c;
}

void VGA_UpdateSpeed(int speed) {
    char str[9] = {'S','P','D',':',' ','0','0','0',' '};
    
    str[5] = (speed / 100) + '0';
    str[6] = ((speed / 10) % 10) + '0';
    str[7] = (speed % 10) + '0';
    
    for(int i = 0; i < 9; i++) {
        write_char(2 + i, 2, str[i]);
    }
}