// UJJWAL CHAUDHARY, 22577, M. Tech. ESE, IISc Bengaluru
// LAB 05

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <./inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

// MACROS

// Ports
#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
//Colors
#define COLOR_GREEN_ON  0x08
#define COLOR_BLUE_ON   0x04
#define COLOR_CYAN_ON   0x0C
#define COLOR_RED_ON    0x02
#define COLOR_YELLOW_ON 0x0A
#define COLOR_MAGENTA_ON 0x06
#define COLOR_WHITE_ON  0x0E
#define NO_COLOR 0x00
// Variables
#define MAX_BLINK_DELAY 1000    // in ms
#define MIN_BLINK_DELAY 100     // in ms
#define DEBOUNCING_DELAY 50     // in ms
#define MAX_BUFFER_SIZE 50      // in bytes
#define BAUD_RATE 115200
// Characters
#define NEW_LINE '\n'
#define ENTER '\r'
#define SPACE ' '
#define TAB '\t'
#define BACKSPACE '\b'
#define NULL_CHAR '\0'
#define TRUE 1
#define FALSE 0
// Conditions
#define SW1 (GPIO_PORTF_DATA_R & 0x10)
#define SW2 (GPIO_PORTF_DATA_R & 0x01)
// 7-Segment Display
#define DISPLAY_NUM_0 0x3F
#define DISPLAY_NUM_1 0x06
#define DISPLAY_NUM_2 0x5B
#define DISPLAY_NUM_3 0x4F
#define DISPLAY_NUM_4 0x66
#define DISPLAY_NUM_5 0x6D
#define DISPLAY_NUM_6 0x7D
#define DISPLAY_NUM_7 0x07
#define DISPLAY_NUM_8 0x7F
#define DISPLAY_NUM_9 0x6F
#define DIGIT_1 0x10
#define DIGIT_2 0x20
#define DIGIT_3 0x40
#define DIGIT_4 0x80
#define SEGMENT_DELAY 1 // in ms
// 4x4 Keypad
#define COL1 (GPIO_PORTC_DATA_R & 0x10)
#define COL2 (GPIO_PORTC_DATA_R & 0x20)
// LCD Display
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define SHIFT_RIGHT 0x06
#define DISPLAY_ON 0x0F
#define BEGINNING_1ST_LINE 0x80
#define BEGINNING_2ND_LINE 0xC0
#define DATA_8_BIT 0x38
#define DATA_4_BIT 0x28
#define INSTRUCTION_RS_0 0
#define DATA_RS_1 1

//GLOBAL VARIABLES

uint8_t delay;
int blink_delay = MAX_BLINK_DELAY;
int select_color = 0;
int color_change_count = 0;
int pause = FALSE;
int reset_flag = 0;
uint8_t color = COLOR_GREEN_ON;
char colors_list[7][8] ={ "green", "blue", "cyan", "red", "yellow", "magenta", "white"};
char command[MAX_BUFFER_SIZE]={NULL_CHAR};
int cmd_char_index = 0;
char version_str[] = "Version 0.01";

//FUNCTION PROTOTYPES

void Initialize();
void delayMs(int n);
void delay_us(int n);
void delay_and_display(int n);
void update_color();
void check_input();
void check_command();
void UARTPrint(char* statement);
void clearBuffer();
void print_commands();
int validate_and_run_cmd_color();
int validate_and_run_cmd_freq();
int validate_and_run_cmd_pause();
int validate_and_run_cmd_resume();
int validate_and_run_cmd_peek();
int validate_and_run_cmd_poke();
void run_command();
void correct_cmd();
int int_to_display_num(int num);
void display_color_code();
void display_color_count();
void check_command();
void LCD_command(int RS, int value);
void LCD_display();
void Version_LCD(char*a);

//MAIN

int main ()
{
    Initialize();
    LCD_display();
    UARTPrint("\n\rWelcome to the LED Blinking System.\n\r");
    print_commands();
    UARTPrint(version_str);
    UARTPrint("\n\r");

    while(1)
    {
        if(!pause)
            GPIO_PORTF_DATA_R = color;

        check_input();
        delay_and_display(blink_delay);
        check_input();

        if(!pause)
            GPIO_PORTF_DATA_R = NO_COLOR;

        check_input();
        delay_and_display(blink_delay);
        check_input();
    }

}

// FUNCTION DEFINATIONS

void Initialize(){
    SYSCTL_RCGC2_R |= 0x00000020;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlockGPIOCR register
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_DEN_R |= PIN0 + PIN1 + PIN2 + PIN3 + PIN4;
    GPIO_PORTF_DIR_R |= PIN1 + PIN2 + PIN3;
    GPIO_PORTF_DIR_R &= (~PIN4); // Set PF4 (SW1) as input
    GPIO_PORTF_DIR_R &= (~PIN0); // Set PF0 (SW2) as input
    GPIO_PORTF_PUR_R |= PIN4 + PIN0; // Enable pull-up resistor for PF4 (SW1) and PF0 (SW2)

    // Initialize UART0

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Configure 7-Segment Display + LCD Display
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xF0);

    // Configure 4x4 Keypad
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutputOD(GPIO_PORTE_BASE, 0x0F);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, 0xF0);
    GPIOPadConfigSet(GPIO_PORTC_BASE, 0xF0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void update_color(){
    uint8_t prev_color = color;
    switch(select_color) {
            case 0:
                color = COLOR_GREEN_ON;
                break;
            case 1:
                color = COLOR_BLUE_ON;
                break;
            case 2:
                color = COLOR_CYAN_ON;
                break;
            case 3:
                color = COLOR_RED_ON;
                break;
            case 4:
                color = COLOR_YELLOW_ON;
                break;
            case 5:
                color = COLOR_MAGENTA_ON;
                break;
            case 6:
                color = COLOR_WHITE_ON;
                break;
        }

    if(prev_color != color){
        color_change_count++;
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++){
        for(j = 0; j < 3180; j++);
    } /* do nothing for 1 ms */
}

void delay_us(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++){
        for(j = 0; j < 3; j++);
    } /* do nothing for 1 us */
}

void delay_and_display(int n){
    int m = n/(3*SEGMENT_DELAY);
    for(int i=0; i<m; i++){
        display_color_code(); // results in 1 SEGMENT_DELAY
        display_color_count(); // results in 2 SEGMENT_DELAY
    }
}

void check_input(){

    if(!COL1 && reset_flag == 0)   // pause/resume
    {
        delay_and_display(DEBOUNCING_DELAY);
        while(!COL1){
            display_color_code();
            display_color_count();
        } // Wait until COL1 is released
        pause = !pause;
        GPIO_PORTF_DATA_R = NO_COLOR;
    }

    if(!COL2)   // reset/play
    {
        delay_and_display(DEBOUNCING_DELAY);
        while(!COL2){
            display_color_code();
            display_color_count();
        } // Wait until COL1 is released
        if(reset_flag==0) // First press
        {
            reset_flag++;
            select_color = 0;
            update_color();
            color_change_count = 0;
            GPIO_PORTF_DATA_R = NO_COLOR;
            blink_delay = MAX_BLINK_DELAY;
            pause = TRUE;
        }else{          // Second press
            pause = FALSE;
            reset_flag = 0;
        }
    }

    if(!SW1 && !pause)
    {
        delay_and_display(DEBOUNCING_DELAY);
        while(!SW1){
            display_color_code();
            display_color_count();
        } // Wait until SW1 is released
        select_color = (select_color + 1)%7;
        update_color();
    }

    if(!SW2 && !pause)
    {
        delay_and_display(DEBOUNCING_DELAY);
        while(!SW2){
            display_color_code();
            display_color_count();
        } // Wait until SW2 is released
        if(blink_delay > MIN_BLINK_DELAY)
        {
            blink_delay = blink_delay/2;
        }
        else
        {
            blink_delay = MAX_BLINK_DELAY;
        }
    }

    check_command();

}

void check_command(){
    if (cmd_char_index < MAX_BUFFER_SIZE) {
        if(UARTCharsAvail(UART0_BASE)) {
            char input_char = UARTCharGet(UART0_BASE);
            if (input_char == ENTER) {
                correct_cmd();
                run_command();
            } else if (input_char == BACKSPACE) {
                UARTCharPut(UART0_BASE, BACKSPACE);
                UARTCharPut(UART0_BASE, SPACE);
                UARTCharPut(UART0_BASE, BACKSPACE);
                if(cmd_char_index!=0)
                    cmd_char_index--;
                command[cmd_char_index] = NULL_CHAR;
            } else if (input_char == TAB) {
                UARTCharPut(UART0_BASE, SPACE);
            } else {
                UARTCharPut(UART0_BASE, input_char);
                command[cmd_char_index] = input_char;
                cmd_char_index++;
            }
        }
    } else {
        UARTPrint("\n\rInput cannot exeed 30 characters. Clearing input buffer.\n\r");
        clearBuffer();
    }
}

void UARTPrint(char* statement) {
    while(*statement) {
        UARTCharPut(UART0_BASE, *(statement++));
    }
}

void clearBuffer() {
    for (int i=0; i<MAX_BUFFER_SIZE; i++) {
        command[i] = NULL_CHAR;
    }
    cmd_char_index = 0;
}

void print_commands(){
    UARTPrint("\n\r [1] color <color name> ; color name must be green, blue, cyan, red, yellow, magenta or white.\
        \n\r [2] blink <blink rate> ; blink rate must be an integer. It is the frequency of LED blinking.\
        \n\r [3] pause ; to pause the LED blinking.\
        \n\r [4] resume ; to resume the LED blinking (Provided LED blinking is paused).\
        \n\r [5] stop ; to stop the LED blinking.\
        \n\r [6] start ; to start the LED blinking (Provided LED blinking is stopped).\
        \n\r [7] peek <address> <no. of bytes> ; to read data from a given address.\
        \n\r [8] poke <address> <no. of bytes> <data> ; to write data to a given address.\
        \n\r");
}

int validate_and_run_cmd_color() {
    char entered_color[8];
    if (sscanf(command, "color%s", entered_color) == 1) {
        for (int i=0; i < 7; i++) {
            if (strcmp(entered_color, colors_list[i]) == 0) {
                select_color = i;
                update_color();
                return TRUE;
            }
        }
    }
    return FALSE;
}

int validate_and_run_cmd_freq() {
    int blink_rate;
    if (sscanf(command, "blink%d", &blink_rate) == 1) {
        int temp_blink_delay = MAX_BLINK_DELAY/blink_rate;
        if(temp_blink_delay > MIN_BLINK_DELAY){
            blink_delay = temp_blink_delay;
            return TRUE;
        }
        else{
            return FALSE;
        }
    }
    return FALSE;
}

int validate_and_run_cmd_pause() {
    if (command[0] == 'p' && command[1] == 'a' && command[2] == 'u' && command[3] == 's' && command[4] == 'e' && command[5] == NULL_CHAR && reset_flag == 0) {
        pause = TRUE;
        GPIO_PORTF_DATA_R = NO_COLOR;
        return TRUE;
    }
    return FALSE;
}

int validate_and_run_cmd_resume() {
    if (command[0] == 'r' && command[1] == 'e' && command[2] == 's' && command[3] == 'u' && command[4] == 'm' && command[5] == 'e' && command[6] == NULL_CHAR && reset_flag == 0) {
        if (pause == TRUE)
        {
            pause = FALSE;
            return TRUE;
        }
    }
    return FALSE;
}

int validate_and_run_cmd_restart(){
    if (command[0] == 's' && command[1] == 't' && command[2] == 'o' && command[3] == 'p' && command[4] == NULL_CHAR) {
        if(reset_flag==0) // First press
        {
            reset_flag++;
            select_color = 0;
            update_color();
            color_change_count = 0;
            GPIO_PORTF_DATA_R = NO_COLOR;
            blink_delay = MAX_BLINK_DELAY;
            pause = TRUE;
            return TRUE;
        }
    } else if (command[0] == 's' && command[1] == 't' && command[2] == 'a' && command[3] == 'r' && command[4] == 't' && command[5] == NULL_CHAR) {
        if(reset_flag) // Second press
        {
            pause = FALSE;
            reset_flag = 0;
            return TRUE;
        }
    }
    return FALSE;
}

int validate_and_run_cmd_peek(){
    if (command[0] == 'p' && command[1] == 'e' && command[2] == 'e' && command[3] == 'k') {

       // adding a space between the address and bytes
       // Eg. command = p   e   e   k   0   x   2   0   0   0   0   0   0   0   1   0
       //               0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
       // For extracting address and bytes, we can use sscanf with format "peek %d %d"
       // Need to make command = peek0x20000000 10
       for(int i=MAX_BUFFER_SIZE-2; i>=14; i--){
            command[i+1] = command[i];
       }
       command[14] = SPACE;
       int address, bytes;
        if (sscanf(command, "peek%x %d", &address, &bytes) == 2) {
            if (address >= 0x20000000 && address <= 0x20008000 && bytes > 0 && bytes <= 100) {
                char data[bytes];
                for (int i=0; i<bytes; i++) {
                    data[i] = *((char*)address + i);
                }
                UARTPrint("\n\rData at given address is: ");
                for (int i=0; i<bytes; i++) {
                    if(data[i]==NULL_CHAR){
                        UARTPrint("_NULL_");
                    }else if (data[i]==NEW_LINE){
                        UARTPrint("_NEW LINE_");
                    }else if (data[i]==ENTER){
                        UARTPrint("_ENTER_");
                    }else if(data[i]==SPACE){
                        UARTPrint("_SPACE_");
                    }else if(data[i]==TAB){
                        UARTPrint("_TAB_");
                    }else if(data[i]==BACKSPACE){
                        UARTPrint("_BACKSPACE_");
                    }else{
                        UARTCharPut(UART0_BASE, data[i]);
                    }
                }
                UARTPrint("\n\r");
                return TRUE;
            }
        }
    }
    return FALSE;
}

int validate_and_run_cmd_poke(){
    if (command[0] == 'p' && command[1] == 'o' && command[2] == 'k' && command[3] == 'e') {
        int address, bytes;
        char data[100];
        if (sscanf(command, "poke%x %d %s", &address, &bytes, data) == 3) {
            if (address >= 0x20000000 && address <= 0x20008000 && bytes > 0 && bytes <= 100) {
                for (int i=0; i<bytes; i++) {
                    *((char*)address + i) = data[i];
                }
                UARTPrint("\n\rData written to given address.\n\r");
                LCD_display();
                return TRUE;
            }
        }
    }
    return FALSE;
}

void run_command() {
    int validate=0;
    switch(command[0])
    {
    case 'c' : {    // color
            validate = validate_and_run_cmd_color();
            clearBuffer();
        }
        break;
    case 'b' : {    // blink
            validate = validate_and_run_cmd_freq();
            clearBuffer();
        }
        break;
    case 'p' : {    // pause , peek , poke
            if (command[1] == 'a') {
                validate = validate_and_run_cmd_pause();
                clearBuffer();
            } else if (command[1] == 'e') {
                validate = validate_and_run_cmd_peek();
                clearBuffer();
            } else if (command[1] == 'o') {
                validate = validate_and_run_cmd_poke();
                clearBuffer();
            }
        }
        break;
    case 'r' : {    // resume
            validate = validate_and_run_cmd_resume();
            clearBuffer();
        }
        break;
    case 's' : {    // restart
            validate = validate_and_run_cmd_restart();
            clearBuffer();
        }
        break;
    default: clearBuffer();
    }

    if(!validate) {
        UARTPrint("\n\rInvalid command. Please enter a valid command.\n\r");
        print_commands();
    }
    clearBuffer();
}

void correct_cmd(){
    char *raw_command = command;
    char correct_cmd[MAX_BUFFER_SIZE];
    int correct_cmd_index = 0;

    while(*raw_command) {
        if (((*raw_command) != SPACE) & ((*raw_command) != TAB) & ((*raw_command) != NULL_CHAR) & ((*raw_command) != BACKSPACE)) {
            correct_cmd[correct_cmd_index] = tolower((*raw_command));
            correct_cmd_index++;
        }
        raw_command++;
    }
    correct_cmd[correct_cmd_index] = NULL_CHAR;
    char correct_cmd_temp[MAX_BUFFER_SIZE];
    // Special case of poke commands
    char correct_cmd2[MAX_BUFFER_SIZE];
    int correct_cmd_index2 = 0;

    if(correct_cmd[0] == 'p' && correct_cmd[1] == 'o' && correct_cmd[2] == 'k' && correct_cmd[3] == 'e') {
        int space_flag = 0;
        correct_cmd2[0] = 'p';
        correct_cmd2[1] = 'o';
        correct_cmd2[2] = 'k';
        correct_cmd2[3] = 'e';
        correct_cmd_index2 = 4;
       for(int i=4; i<MAX_BUFFER_SIZE; i++) {
           if(command[i] != SPACE){
                correct_cmd2[correct_cmd_index2++] = command[i];
           }else if(space_flag <= 1){
            correct_cmd2[correct_cmd_index2++] = SPACE;
            space_flag++;
            while(command[i] == SPACE){
                i++;
            }
            i--;
           }else {
                correct_cmd2[correct_cmd_index2++] = command[i];
           }
       }
        correct_cmd2[correct_cmd_index2] = NULL_CHAR;
        
        if(sscanf(correct_cmd, " %s", correct_cmd_temp) == 1) {
        strcpy(correct_cmd, correct_cmd_temp);
        }

        strcpy(command, correct_cmd2);
        UARTPrint("\n\r");
        return;
    }

    strcpy(command, correct_cmd);
    UARTPrint("\n\r");
}

int int_to_display_num(int num){
    switch(num){
        case 0: return DISPLAY_NUM_0;
        case 1: return DISPLAY_NUM_1;
        case 2: return DISPLAY_NUM_2;
        case 3: return DISPLAY_NUM_3;
        case 4: return DISPLAY_NUM_4;
        case 5: return DISPLAY_NUM_5;
        case 6: return DISPLAY_NUM_6;
        case 7: return DISPLAY_NUM_7;
        case 8: return DISPLAY_NUM_8;
        case 9: return DISPLAY_NUM_9;
        default: return DISPLAY_NUM_0;
    }
}

void display_color_code(){
    GPIO_PORTB_DATA_R = int_to_display_num(select_color);
    GPIO_PORTA_DATA_R = DIGIT_1;
    delayMs(SEGMENT_DELAY);
}

void display_color_count(){
    color_change_count = color_change_count%100; // keeping color count within 2 digits
    int once_digit = color_change_count%10;
    int tens_digit = color_change_count/10;
    GPIO_PORTB_DATA_R = int_to_display_num(once_digit);
    GPIO_PORTA_DATA_R = DIGIT_2;
    delayMs(SEGMENT_DELAY);
    GPIO_PORTB_DATA_R = int_to_display_num(tens_digit);
    GPIO_PORTA_DATA_R = DIGIT_3;
    delayMs(SEGMENT_DELAY);
}

void LCD_command(int RS, int value){
    if(RS == INSTRUCTION_RS_0){
        GPIO_PORTA_DATA_R &= ~0x40; // A6=0 RS=0
        GPIO_PORTB_DATA_R = value;
        GPIO_PORTA_DATA_R |= 0x80;  // A7=1 En=1
        delayMs(0);
        GPIO_PORTA_DATA_R &= ~0x80; // En=0
        delayMs(2);
    }else{
        GPIO_PORTA_DATA_R |= 0x40;  // RS=1
        GPIO_PORTB_DATA_R = value;
        GPIO_PORTA_DATA_R |= 0x80;  // En=1
        delayMs(0);
        GPIO_PORTA_DATA_R &= ~0x80; // En=0
        delay_us(50);
    }
}

void LCD_display()
{
    // Set up the LCD
    LCD_command(INSTRUCTION_RS_0, DATA_8_BIT);
    LCD_command(INSTRUCTION_RS_0, CLEAR_DISPLAY);
    LCD_command(INSTRUCTION_RS_0, DISPLAY_ON);
    LCD_command(INSTRUCTION_RS_0, RETURN_HOME);
    // 1st line
    LCD_command(INSTRUCTION_RS_0, BEGINNING_1ST_LINE);
    Version_LCD(version_str);
}

void Version_LCD(char*a){

   while(*a != '\0'){
        LCD_command(DATA_RS_1,*a);
        ++a;
    }

}
