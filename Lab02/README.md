# Assignment 02

This assignment is for the partial grading of the course E3-257.

# RGB LED Controller with TIVA C Series LaunchPad

This project implements a simple RGB LED controller using the TIVA C Series LaunchPad. The program allows the user to cycle through different colors by pressing a button (SW1), and it also provides the option to change the blinking speed of the LED by pressing another button (SW2).

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Macros and Definitions](#macros-and-definitions)
- [How to Use](#how-to-use)
- [Contributing](#Author)

## Features

- LED colors: Green, Blue, Cyan, Red, Yellow, Magenta, and White.
- Adjust the blinking speed with SW2.
- Debouncing mechanism for button presses.

## Hardware Requirements

- TIVA C Series LaunchPad.
- RGB LED connected to pins PF1, PF2, and PF3.
- Two push buttons (SW1 and SW2) connected to pins PF4 and PF0.

## Macros and Definitions

- **Pins and Clock Macros:**
  - `PIN0`, `PIN1`, `PIN2`, `PIN3`, `PIN4`: Bit masks for individual pins.
- **Color Macros:**
  - `COLOR_GREEN_ON`, `COLOR_BLUE_ON`, `COLOR_CYAN_ON`, `COLOR_RED_ON`, `COLOR_YELLOW_ON`, `COLOR_MAGENTA_ON`, `COLOR_WHITE_ON`: RGB LED color configurations.
- **Blinking Delay Limits:**
  - `MAX_BLINK_DELAY`: Maximum delay for LED blinking.
  - `MIN_BLINK_DELAY`: Minimum delay for LED blinking.

## How to Use

1. Connect the RGB LED to pins PF1, PF2, and PF3.
2. Connect push button SW1 to pin PF4 and SW2 to pin PF0.
3. Flash the provided code onto the TIVA C Series LaunchPad.
4. Run the project, and you can cycle through colors with SW1 and adjust blinking speed with SW2.

## Author

[UJJWAL CHAUDHARY, M. Tech. ESE 2023-25, IISc Bengaluru](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)