#include "system.h"
#include "uart.h"
#include "vga.h"

int main() {
    VGA_DrawRadar();
    System_Init();
    Rover_Init();

    while (!System_IsAborted()) {
        int speed = Rover_GetSpeed();

        if (speed < 10) {
            Rover_Accelerate();
        }
        else if (speed > 50) {
            Rover_Brake();
        }
        VGA_UpdateSpeed(speed);
    }
    System_Scram();
}