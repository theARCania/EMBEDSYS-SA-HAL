#include "led.h"

#define LED 0xff200000

void LED_Scram() {
    int delay;
    while (1) {
        delay = 696960; while (delay--) *((volatile int*)LED) = 0xff;
        delay = 696960; while (delay--) *((volatile int*)LED) = 0;
    }
}