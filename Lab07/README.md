# Lab 7 Assignment - Embedded Systems (E3-257)

## Description
This C program is part of the Lab 7 assignment for the Embedded Systems course (E3-257). This program is designed to implement Stop watch DOWN timer using system clock interrupt with blinking of an LED TM4C123GH6PM microcontroller. The program supports UART communication for user commands, allowing users to pause, resume, stop and restart the stopwatch. Apart from UART SW1 and SW2 can also be used to control the watch.
Motor speed and LED blinking speed is controlled by the potentiometer in the EduARM4 Trainer Board.

## Hardware Requirements
- **Microcontroller:** TM4C123GH6PM
- **Peripherals:** EduARM4 Trainer Board Embedded Lab DESE IISc v1.0.4a, DC Motor, 9v battery, L298N Motor Driver

## Dependencies
- **TivaWare Peripheral Driver Library:** Used for GPIO and UART configurations.
- **UART0:** Configured for UART communication with a baud rate of 115200.
- **GPIO:** Utilized for LED control (different colors) and 7-Segment Display
- **ADC and PWM:** Used ADC to monitor potentiometer and PWM to control motor speed.

## Macros
- **Ports:** Definitions for different GPIO pins.
- **Colors:** Macros for different LED colors.
- **Variables:** Definitions for maximum blink delay, minimum blink delay, debouncing delay, maximum buffer size, baud rate, and more.
- **Characters:** Definitions for special characters and boolean values.
- **Conditions:** Macros for switch conditions and keypad columns.
- **7-Segment Display:** Definitions for 7-segment display configurations.

## Global Variables

- `delay`: The current delay for LED blinking.
- `pwm_value`: The current PWM value for controlling LED brightness.
- `blink_delay`: The delay between LED blink changes.
- `blink_time`: The current time for LED blinking.
- `select_color`: The selected color for LED blinking.
- `color_change_count`: The count of color changes for LED blinking.
- `LED_blink`: Flag indicating whether the LED should blink.
- `pause`: Flag indicating whether the stop watch is paused.
- `reset_flag`: Flag indicating whether the stop watch has been reset.
- `color`: The current color for LED blinking.
- `command`: The command buffer for UART input.
- `cmd_char_index`: The index of the next character in the command buffer.
- `time`: The current time for the stop watch.
- `LED_digit`: The current digit for the 7-segment display.
- `LED_digit_delay`: The delay between 7-segment display digit changes.
- `display_pwm_flag`: Flag indicating whether to display PWM value instead of time.
- `pwm_display_timer`: Timer for displaying PWM value.

## Functions

- `Initialize()`: Initializes the hardware components and peripherals.
- `update_color()`: Updates the LED color based on the selected color.
- `check_input()`: Checks for user input via UART.
- `check_command()`: Checks the command buffer for valid commands.
- `UARTPrint(char* statement)`: Prints a string via UART.
- `clearBuffer()`: Clears the command buffer.
- `print_commands()`: Prints the available commands via UART.
- `validate_and_run_cmd_pause()`: Validates and runs the pause command.
- `validate_and_run_cmd_resume()`: Validates and runs the resume command.
- `validate_and_run_cmd_restart()`: Validates and runs the restart command.
- `run_command()`: Executes the command based on user input.
- `correct_cmd()`: Corrects the command buffer to lowercase and removes spaces.
- `int_to_display_num(int num)`: Converts an integer to a 7-segment display number.
- `display_time()`: Displays the current time on the 7-segment display.
- `display_PWM()`: Displays the current PWM value on the 7-segment display.
- `SysTick_Handler()`: Interrupt handler for the system tick timer.
- `GPIOPortF_Handler()`: Interrupt handler for the GPIO Port F buttons.

## Main Function
The main function initializes the microcontroller and peripherals, entering an infinite loop. Inside the loop, it checks for UART commands.

## User Commands
The program supports the following UART commands:
1. `pause`: Pause the Stop Watch.
2. `resume`: Resume the Stop Watch (if paused).
3. `stop`: Stop the Stop Watch.
4. `start`: Start the Stop Watch (if stopped).
5. `tictactoe`: To start Tic-Tac-Toe game.

## Switch/Key Control
  - Press SW1 to Stop Watch Pause/Resume.
  - Press SW2 to Stop Watch Stop/Start.
  - Potentiometer to control LED blink speed and motor speed.

## Author

[UJJWAL CHAUDHARY, M. Tech. ESE 2023-25, IISc Bengaluru](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)
