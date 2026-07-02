# EMBEDSYS-SA-HAL

This project provides a hardware abstraction layer (HAL) and basic control loop for a bare-metal embedded system application. It interacts with various memory-mapped peripherals to control a rover and present information via a VGA display and 7-segment displays.

## Features

*   **Rover Control (UART):** Uses UART communication to control a rover. Supports acceleration, braking, and reading the current speed.
*   **VGA Display:** Includes a VGA driver capable of drawing a radar display using Bresenham's circle algorithm. It also displays the current rover speed as text on the screen.
*   **Interrupt-Driven Hardware:**
    *   **Timer Interrupts:** Drives a 9-to-0 countdown displayed on a memory-mapped 7-segment display.
    *   **Pushbutton Interrupts:** Supports a hardware interrupt for a system abort ("Scram" sequence), which triggers LED blinking and halts the rover operation.
*   **Hardware Abstraction Layer (HAL):** Organizes code into logical modules (`uart.c`, `vga.c`, `led.c`, `system.c`) that interface with memory-mapped peripherals, abstracting the low-level memory addresses and interrupt controller (GIC) configuration from the main application logic.

## Project Structure

*   `main.c`: The main entry point and control loop, initializing the system, updating the VGA radar, managing rover speed, and handling the abort sequence.
*   `system.c` / `system.h`: Handles core system initialization, including the General Interrupt Controller (GIC), interrupt handlers, and the timer.
*   `uart.c` / `uart.h`: Manages UART communication for rover control.
*   `vga.c` / `vga.h`: Provides pixel-level and character-level drawing functions for the VGA output.
*   `led.c` / `led.h`: Controls memory-mapped LEDs, primarily used for the system scram sequence.
# EMBEDSYS-SA-HAL
