# Assignment 02

This assignment is for the partial grading of the course E3-257.

# Blinking LED Project Readme

This project is designed for the TM4C123GH6PM microcontroller to control the blinking of an LED with various colors using push buttons.

## Table of Contents

- [Introduction](#introduction)
- [Requirements](#requirements)
- [Hardware Setup](#hardware-setup)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Author](#author)

## Introduction

This project utilizes the TM4C123GH6PM microcontroller to control the blinking of an LED with different colors. The blinking pattern and LED color can be changed using push buttons (SW1 and SW2).

## Requirements

- **Hardware:** TM4C123GH6PM microcontroller, LED, push buttons (SW1 and SW2)
- **Software:** TivaWare Peripheral Driver Library

## Hardware Setup

1. Connect the LED to the appropriate pins on the TM4C123GH6PM microcontroller.
2. Connect push buttons SW1 and SW2 to the designated pins on the microcontroller.
3. Ensure proper power supply and connections.

## Usage

1. Clone or download the project repository.
2. Open the project in your preferred IDE (Integrated Development Environment).
3. Build and flash the code to the TM4C123GH6PM microcontroller.
4. Run the project, and observe the LED blinking behavior.
5. Press SW1 to change the LED color.
6. Press SW2 to adjust the blinking delay.

## Code Structure

The code is organized into the following sections:

- **Macro Definitions:** Defines pins, colors, and various constants used in the code.
- **Global Variables:** Declarations of global variables used in the main program.
- **Functions:**
  - `delayMs`: Implements a delay in milliseconds.
  - `delayMs_check`: Adds functionality to `delayMs` by checking for button presses during the delay.
  - `update_color`: Updates the LED color based on the selected color.
  - `main`: The main program that controls the LED blinking based on button inputs.

## Author

[UJJWAL CHAUDHARY, M. Tech. ESE 2023-25, IISc Bengaluru](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)