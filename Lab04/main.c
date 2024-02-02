// UJJWAL CHAUDHARY, 22577, M. Tech. ESE, IISc Bengaluru
// LAB 03

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
#define MAX_BUFFER_SIZE 30      // in bytes
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

//GLOBAL VARIABLES

uint8_t delay;
int blink_delay = MAX_BLINK_DELAY;
int select_color = 0;
int color_change_count = 0;
uint8_t color = COLOR_GREEN_ON;
char colors_list[7][8] ={ "green", "blue", "cyan", "red", "yellow", "magenta", "white"};
char command[MAX_BUFFER_SIZE]={NULL_CHAR};
int cmd_char_index = 0;

//FUNCTION PROTOTYPES

void Initialize();
void delayMs(int n);
void delay_and_display();
void update_color();
void check_input();
void check_command();
void UARTPrint(char statement[]);
void clearBuffer();
int validate_cmd_color();
int validate_cmd_freq();
void run_command();
void correct_cmd();
int int_to_display_num(int num);
void display_color_code();
void display_color_count();

//MAIN

int main ()
{   
    Initialize();

    while(1)
    {   
        GPIO_PORTF_DATA_R = color;
        check_input();
        delay_and_display(blink_delay);
        check_input();
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

    // Configure 7-Segment Display
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xFF);
}

void update_color(){
    color_change_count++;
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
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++){
        for(j = 0; j < 3180; j++);
    } /* do nothing for 1 ms */
}

void delay_and_display(int n){
    int m = n/(3*SEGMENT_DELAY);
    for(int i=0; i<m; i++){
        display_color_code(); // results in 1 SEGMENT_DELAY
        display_color_count(); // results in 2 SEGMENT_DELAY
    }
}

void check_input(){
    if(!SW1)
    {
        delay_and_display(DEBOUNCING_DELAY);
        while(!SW1){
            display_color_code();
            display_color_count();
        } // Wait until SW1 is released
        select_color = (select_color + 1)%7;
        update_color();
    }

    if(!SW2)
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
            }

            if (input_char != TAB) {
                UARTCharPut(UART0_BASE, input_char);
            } else {
                UARTCharPut(UART0_BASE, SPACE);
            }

            if (input_char == BACKSPACE) {
                UARTCharPut(UART0_BASE, SPACE);
                UARTCharPut(UART0_BASE, BACKSPACE);
                cmd_char_index--;
                command[cmd_char_index] = NULL_CHAR;
            }

            command[cmd_char_index] = tolower(input_char);
            cmd_char_index++;
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

void run_command() {
    int validate=0;
    switch(command[0])
    {
    case 'c' : {validate = validate_and_run_cmd_color();
                clearBuffer();
                }
             break;
    case 'b' : {validate = validate_and_run_cmd_freq();
                clearBuffer();
                }
             break;
    default: clearBuffer();
    }

    if(!validate) {
        UARTPrint("\n\rGiven command is invalid.\n\rCommand Help\n\rValid commands are,\
              \n\r [1] color <color name> ; color name must be green, blue, cyan, red, yellow, magenta or white.\
              \n\r [2] blink <blink rate> ; blink rate must be an integer \n");
    }
    clearBuffer();
}

void correct_cmd(){
    char *raw_command = command;
    char correct_cmd[MAX_BUFFER_SIZE];
    int correct_cmd_index = 0;
    while(*raw_command) {
        if (((*raw_command) != SPACE) & ((*raw_command) != TAB) & ((*raw_command) != NULL_CHAR) & ((*raw_command) != BACKSPACE)) {
            correct_cmd[correct_cmd_index] = (*raw_command);
            correct_cmd_index++;
        }
        raw_command++;
    }
    correct_cmd[correct_cmd_index] = NULL_CHAR;
    char correct_cmd_temp[MAX_BUFFER_SIZE];

    if(sscanf(correct_cmd, " %s", correct_cmd_temp) == 1) {
        strcpy(correct_cmd, correct_cmd_temp);
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
    }
}

void display_color_code(){
    GPIO_PORTB_DATA_R = int_to_display_num(select_color);
    GPIO_PORTA_DATA_R = DIGIT_1;
    delayMs(SEGMENT_DELAY);
}

void display_color_count(){
    int once_digit = color_change_count%10;
    int tens_digit = color_change_count/10;
    GPIO_PORTB_DATA_R = int_to_display_num(once_digit);
    GPIO_PORTA_DATA_R = DIGIT_3;
    delayMs(SEGMENT_DELAY);
    GPIO_PORTB_DATA_R = int_to_display_num(tens_digit);
    GPIO_PORTA_DATA_R = DIGIT_4;
    delayMs(SEGMENT_DELAY);
}