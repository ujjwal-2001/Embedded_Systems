# Lab 3 Assignment - Embedded Systems (E3-257)

## Introduction

This C program is part of the Lab 3 assignment for the Embedded Systems course (E3-257). It is designed to run on a TM4C123GH6PM microcontroller and control the color and blink rate of an LED. The program leverages switches and UART commands for user interaction.

## Features

- **Color Control**: Change LED color using a switch (SW1).
- **Blink Rate Adjustment**: Adjust LED blink rate using another switch (SW2).
- **UART Commands**: Use UART commands for advanced control over LED color and blink rate.
- **Valid Colors**: Green, Blue, Cyan, Red, Yellow, Magenta, White.
- **Command Format for Color Change**: `color <color_name>`
- **Command Format for Blink Rate Change**: `blink <blink_rate>`

## Macros

### Ports
- `PIN0`, `PIN1`, `PIN2`, `PIN3`, `PIN4`: Represent bit positions for different pins.
  
### Colors
- `COLOR_GREEN_ON`, `COLOR_BLUE_ON`, `COLOR_CYAN_ON`, `COLOR_RED_ON`, `COLOR_YELLOW_ON`, `COLOR_MAGENTA_ON`, `COLOR_WHITE_ON`, `NO_COLOR`: Macros for controlling LED colors.

### Variables
- `MAX_BLINK_DELAY`, `MIN_BLINK_DELAY`, `DEBOUNCING_DELAY`, `MAX_BUFFER_SIZE`, `BAUD_RATE`: Constants for configuration.

### Characters
- `NEW_LINE`, `ENTER`, `SPACE`, `TAB`, `BACKSPACE`, `NULL_CHAR`: Constants representing special characters.

### Conditions
- `SW1`, `SW2`: Macros for checking the state of switches.

## Getting Started

Follow these steps to get started with the program:

1. **Connect the Hardware**: Ensure the TM4C123GH6PM microcontroller is correctly connected.
2. **Build and Flash**: Build the program and flash it onto the microcontroller.
3. **Terminal Setup**: Run a terminal program (e.g., PuTTY) with a baud rate of 115200 to interact with the microcontroller.

## Usage

- **Switch Control**:
  - Press SW1 to cycle through LED colors.
  - Press SW2 to adjust the LED blink rate.

- **UART Commands**:
  - Send UART commands for precise control.
  - Valid color change command: `color <color_name>`
  - Valid blink rate change command: `blink <blink_rate>`

## Command Help

If you're using UART commands, refer to the following guidelines:

- **Color Command**: `color <color_name>`
  - `<color_name>` should be one of the following: green, blue, cyan, red, yellow, magenta, white.

- **Blink Rate Command**: `blink <blink_rate>`
  - `<blink_rate>` is an integer value representing the desired blink rate.

## Examples

- Change LED color to blue: `color blue`
- Set LED blink rate to 5: `blink 5`

## Author

[UJJWAL CHAUDHARY, M. Tech. ESE 2023-25, IISc Bengaluru](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)
