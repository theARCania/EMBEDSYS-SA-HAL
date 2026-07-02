#include "uart.h"

#define CWDATA 0xff211020
#define CWCTRL 0xff211024

static int readUart() {
    int data;
    do {
        data = *(volatile int*)(CWDATA);
    }
    while ((data & 0x8000) == 0);
    return data & 0xff;
}

static void writeUart(int c) {
    while (((*(volatile int*)(CWCTRL)) & 0xffff0000) == 0);
    *(volatile int*)(CWDATA) = c;
}

void Rover_Init() {
    while (*(volatile int*)(CWDATA) & 0x8000);
}

int Rover_GetSpeed() {
    writeUart(0x02);
    readUart();
    readUart();
    return readUart();
}

void Rover_Accelerate() {
    writeUart(0x04);
    writeUart(127);
}

void Rover_Brake() {
    writeUart(0x04);
    writeUart(0);
}