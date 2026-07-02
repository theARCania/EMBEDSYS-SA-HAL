#include "system.h"
#include "led.h"

#define BUTTON 0xff200050
#define DISPLAY 0xff200020
#define TIMER 0xfffec600
#define PERIOD 200000000

static volatile bool Abort = 0;
static unsigned int irq_stack[256];
static volatile int currCount = 9;
static const int lookupTable[10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f};

void __attribute__((naked, section(".vectors"))) vector_table(void) {
    asm volatile ("B _start\n B _start\n B _start\n B _start\n B _start\n B _start\n B __cs3_isr_irq\n B _start");
}

void __attribute__((interrupt)) __cs3_isr_irq(void) {
    int irq_id = *((volatile int *)0xFFFEC10C);
    
    if (irq_id == 73) {
        *((volatile int *)(BUTTON + 0xC)) = 0xF;
        Abort = 1;
    }
    if (irq_id == 29) {
        if (!Abort && currCount > -1) {
            *(volatile int*)(DISPLAY) = lookupTable[currCount--];
        }
        *(volatile int*)(TIMER+12) = 1;
    }
    
    *((volatile int *)0xFFFEC110) = irq_id;
}

static void setIRQstack() {
    asm volatile ("msr cpsr_c, %0\n mov sp, %1\n msr cpsr_c, %2" : : "r"(0xd2), "r"(&irq_stack[255]), "r"(0xd3));
}

static void enableInterrupts() {
    int status;
    asm("mrs %0, cpsr" : "=r" (status));
    asm("msr cpsr_c, %0" : : "r" (status & ~0x80));
}

static void configureGIC() {
    *((volatile int *)0xFFFED848) = 0x100;
    *((volatile int *)0xFFFED108) = 0x300;
    *((volatile int *)0xFFFED100) |= 0x20000000;
    *((volatile int *)0xFFFEC104) = 0xFFFF;
    *((volatile int *)0xFFFEC100) = 1;
    *((volatile int *)0xFFFED000) = 1;
}

void System_Init() {
    setIRQstack();
    configureGIC();
    *((volatile int *)(BUTTON + 8)) = 1; 
    
    enableInterrupts();
    
    *(volatile int*) (TIMER) = PERIOD; 
    *(volatile int*) (TIMER+8) = 0x7; 
}

bool System_IsAborted() {
    return Abort;
}

void System_Scram() {
    *(volatile int*)(TIMER+8) = 0; 
    LED_Scram();
}