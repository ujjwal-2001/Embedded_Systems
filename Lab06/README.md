# Lab 6 Assignment - Embedded Systems (E3-257)

## Description
This C program is part of the Lab 6 assignment for the Embedded Systems course (E3-257). This program is designed to implement Stop watch timer using system clock interrupt with blinking of an LED, LCD Display and a Tic-Tac-Toe game (via 4x4 key pad) on a TM4C123GH6PM microcontroller. The program supports UART communication for user commands, allowing users to pause, resume, stop and restart the stopwatch. Apart from UART SW1 and SW2 can also be used to control the watch.

## Hardware Requirements
- **Microcontroller:** TM4C123GH6PM
- **Peripherals:** EduARM4 Trainer Board, Embedded Lab, DESE, IISc v1.0.4a

## Dependencies
- **TivaWare Peripheral Driver Library:** Used for GPIO and UART configurations.
- **UART0:** Configured for UART communication with a baud rate of 115200.
- **GPIO:** Utilized for LED control (different colors), 7-Segment Display, LCD Display  and 4x4 Keypad.

## Macros
- **Ports:** Definitions for different GPIO pins.
- **Colors:** Macros for different LED colors.
- **Variables:** Definitions for maximum blink delay, minimum blink delay, debouncing delay, maximum buffer size, baud rate, and more.
- **Characters:** Definitions for special characters and boolean values.
- **Conditions:** Macros for switch conditions and keypad columns.
- **7-Segment Display:** Definitions for 7-segment display configurations.
- **4x4 Keypad:** Macros for keypad columns.
- **LCD Display:** Macros for the LCD Instructions.

## Global Variables

### Delay Variables
- `delay`, `blink_delay`, `blink_time`, `LED_blink`, `pause`, `reset_flag`: Variables for controlling delays and LED blinking.

### Color Variables
- `select_color`, `color_change_count`, `color`: Variables for managing LED colors.

### Command Variables
- `command`, `cmd_char_index`: Variables for storing and managing user commands.

### Time Variables
- `time`: Variable for storing elapsed time.

### LED Variables
- `LED_digit`, `LED_digit_delay`: Variables for controlling the 7-segment display.

### State Variables
- `state`, `state_str`: Variables for managing the state of the system.

### Tic-Tac-Toe Variables
- `TicTacToe`, `TicTacToe_turn`, `TicTacToe_game`, `TicTacToe_moves`, `TicTacToe_winner`: Variables for implementing the Tic-Tac-Toe game.


## Functions
### Initialize()
- **Description**: Initializes the microcontroller and peripherals.
- **Parameters**: None.
- **Return**: None.

### delayMs(int n)
- **Description**: Delays execution by n milliseconds.
- **Parameters**: 
  - `n` : Number of milliseconds to delay.
- **Return**: None.

### delay_us(int n)
- **Description**: Delays execution by n microseconds.
- **Parameters**: 
  - `n` : Number of microseconds to delay.
- **Return**: None.

### update_color()
- **Description**: Updates the LED color based on user input.
- **Parameters**: None.
- **Return**: None.

### check_input()
- **Description**: Checks user input from the keypad.
- **Parameters**: None.
- **Return**: None.

### check_command()
- **Description**: Checks user commands from UART input.
- **Parameters**: None.
- **Return**: None.

### UARTPrint(char* statement)
- **Description**: Prints a string over UART.
- **Parameters**: 
  - `statement` : The string to print.
- **Return**: None.

### clearBuffer()
- **Description**: Clears the input buffer.
- **Parameters**: None.
- **Return**: None.

### print_commands()
- **Description**: Prints the available commands over UART.
- **Parameters**: None.
- **Return**: None.

### validate_and_run_cmd_pause()
- **Description**: Validates and executes the pause command.
- **Parameters**: None.
- **Return**: Returns 1 if successful, 0 otherwise.

### validate_and_run_cmd_resume()
- **Description**: Validates and executes the resume command.
- **Parameters**: None.
- **Return**: Returns 1 if successful, 0 otherwise.

### validate_and_run_cmd_tictactoe()
- **Description**: Validates and executes the tictactoe command.
- **Parameters**: None.
- **Return**: Returns 1 if successful, 0 otherwise.

### initialize_tictactoe()
- **Description**: Initializes the Tic-Tac-Toe game.
- **Parameters**: None.
- **Return**: None.

### print_tictactoe()
- **Description**: Prints the current state of the Tic-Tac-Toe game.
- **Parameters**: None.
- **Return**: None.

### check_winner_tictactoe()
- **Description**: Checks for a winner in the Tic-Tac-Toe game.
- **Parameters**: None.
- **Return**: None.

### run_command()
- **Description**: Runs the user command.
- **Parameters**: None.
- **Return**: None.

### correct_cmd()
- **Description**: Corrects the user command for consistency.
- **Parameters**: None.
- **Return**: None.

### int_to_display_num(int num)
- **Description**: Converts an integer to the corresponding 7-segment display value.
- **Parameters**: 
  - `num` : The integer to convert.
- **Return**: The 7-segment display value.

### display_time()
- **Description**: Displays the elapsed time on the 7-segment display.
- **Parameters**: None.
- **Return**: None.

### SysTick_Handler()
- **Description**: Interrupt handler for the SysTick timer.
- **Parameters**: None.
- **Return**: None.

### GPIOPortF_Handler()
- **Description**: Interrupt handler for GPIO Port F.
- **Parameters**: None.
- **Return**: None.

### update_state()
- **Description**: Updates the LCD display with the current state.
- **Parameters**: None.
- **Return**: None.

### LCD_command(int RS, int value)
- **Description**: Sends a command to the LCD display.
- **Parameters**: 
  - `RS` : The value of the RS pin (0 or 1).
  - `value` : The command value to send.
- **Return**: None.

### LCD_data(char data)
- **Description**: Sends data to be displayed on the LCD.
- **Parameters**: 
  - `data` : The character data to display.
- **Return**: None.

## Main Function
The main function initializes the microcontroller and peripherals, entering an infinite loop. Inside the loop, it checks for UART commands and Tic-Tac-Toe Key press.

## User Commands
The program supports the following UART commands:
3. `pause`: Pause the Stop Watch.
4. `resume`: Resume the Stop Watch (if paused).
5. `stop`: Stop the Stop Watch.
6. `start`: Start the Stop Watch (if stopped).
7. `tictactoe`: To start Tic-Tac-Toe game.

## Switch/Key Control
  - Press SW1 to Stop Watch Pause/Resume.
  - Press SW2 to Stop Watch Stop/Start.
  - First 3 rows and Columns of 4x4 Key pad for Tic-Tac-Toe 

## Author

[UJJWAL CHAUDHARY, M. Tech. ESE 2023-25, IISc Bengaluru](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)
