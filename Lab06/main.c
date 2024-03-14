// UJJWAL CHAUDHARY, 22577, M. Tech. ESE, IISc Bengaluru
// LAB 06

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
#define TYPICAL_BLINK_DELAY 500 // in ms
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
#define SEGMENT_DELAY 4 // in ms
// 4x4 Keypad
#define COL1 (!(GPIO_PORTC_DATA_R & 0x10))
#define COL2 (!(GPIO_PORTC_DATA_R & 0x20))
#define COL3 (!(GPIO_PORTC_DATA_R & 0x40))
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
int blink_delay = TYPICAL_BLINK_DELAY;
int blink_time = 0;
int select_color = 0;
int color_change_count = 0;
int LED_blink = 0;
int pause = FALSE;
int reset_flag = 0;
int state = 0;
uint8_t color = COLOR_GREEN_ON;
char command[MAX_BUFFER_SIZE]={NULL_CHAR};
int cmd_char_index = 0;
unsigned long long time = 0;
int LED_digit = 1;
int LED_digit_delay = 0;
char state_str[] = "Timer Running";
int TicTacToe[3][3];
int TicTacToe_turn = 0;
int TicTacToe_game = FALSE;
int TicTacToe_moves = 0;
int TicTacToe_winner = 0;

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
int validate_and_run_cmd_pause();
int validate_and_run_cmd_resume();
int validate_and_run_cmd_tictactoe();
void initialize_tictactoe();
void print_tictactoe();
void check_winner_tictactoe();
void run_command();
void correct_cmd();
int int_to_display_num(int num);
void display_time();
void check_command();
void LCD_command(int RS, int value);
void LCD_display();
void State_LCD(char*a);
void update_state();

//MAIN

int main ()
{
    Initialize();
    UARTPrint("\n\rWelcome to the Stop Watch System.\n\r");
    print_commands();
    UARTPrint("\n\r");
    LCD_display();

    while(1)
    {
        
        if(LED_blink)
            GPIO_PORTF_DATA_R = color;
        else
            GPIO_PORTF_DATA_R = NO_COLOR;
        

        display_time();
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
    
    // Initialize UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Configure 7-Segment Display Pins + LCD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xF0);

    // Configure SW1 ans SW2 Interrupts
    GPIO_PORTF_PUR_R = PIN0 + PIN4; /* enable pull up for pin 0 and 4 */        

    GPIO_PORTF_IS_R &= ~0x11;       /*  PF is edge-sensitive */
    GPIO_PORTF_IBE_R &= ~0x11;      /*  PF is not both edges */
    GPIO_PORTF_IEV_R &= ~0x11;      /*  PF falling edge event */
    GPIO_PORTF_ICR_R = 0x11;        /*  Clear flag */
    GPIO_PORTF_IM_R |= 0x11;        /*  arm interrupt on PF */
    
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000; /*  priority 5 for Port F*/
    NVIC_EN0_R = 0x40000000;        /*  Enable interrupt 30 in NVIC */

    NVIC_ST_RELOAD_R = 16000;           //Load timer value - interrupt every 1ms
    NVIC_ST_CTRL_R = 7;                 //Enable SYSTICK interrupt
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x0FFFFFFF) | 0x20000000; /*  priority 1 for SYSTICK*/

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

void check_input(){

    check_command();

    if(TicTacToe_game){
        int row = -1;
        int col = -1;
        if(COL1){
            delayMs(DEBOUNCING_DELAY);
            col = 0;
            GPIO_PORTE_DATA_R = 0x0E;
            delayMs(2);
            row  = (COL1)?0:-1;
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0D;
                delayMs(2);
                row  = (COL1)?1:-1;
            }
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0B;
                delayMs(2);
                row  = (COL1)?2:-1;
            }
        }else if(COL2){
            delayMs(DEBOUNCING_DELAY);
            col = 1;
            GPIO_PORTE_DATA_R = 0x0E;
            delayMs(2);
            row  = (COL2)?0:-1;
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0D;
                delayMs(2);
                row  = (COL2)?1:-1;
            }
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0B;
                delayMs(2);
                row  = (COL2)?2:-1;
            }
        }else if(COL3){
            delayMs(DEBOUNCING_DELAY);
            col = 2;
            GPIO_PORTE_DATA_R = 0x0E;
            delayMs(2);
            row  = (COL3)?0:-1;
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0D;
                delayMs(2);
                row  = (COL3)?1:-1;
            }
            if(row==-1){
                GPIO_PORTE_DATA_R = 0x0B;
                delayMs(2);
                row  = (COL3)?2:-1;
            }
        }

        GPIO_PORTE_DATA_R = 0x00;

        if(row != -1 && col != -1){
            if(TicTacToe[row][col] == 0){
                TicTacToe[row][col] = TicTacToe_turn?1:-1;
                TicTacToe_turn = !TicTacToe_turn;
                print_tictactoe();
                TicTacToe_moves++;
                if(TicTacToe_moves>=5){
                    check_winner_tictactoe();
                }
            }else{
                UARTPrint("\n\rInvalid move. Please try again.");
            }
        }
    }

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
    UARTPrint("\n\r [1] pause ; to pause the LED blinking.\
        \n\r [2] resume ; to resume the LED blinking (Provided LED blinking is paused).\
        \n\r [3] stop ; to stop the LED blinking.\
        \n\r [4] start ; to start the LED blinking (Provided LED blinking is stopped).\
        \n\r [5] tictactoe ; to start Tic-Tac-Toe game.\
        \n\r");
}

int validate_and_run_cmd_pause() {
    if (command[0] == 'p' && command[1] == 'a' && command[2] == 'u' && command[3] == 's' && command[4] == 'e' && command[5] == NULL_CHAR && reset_flag == 0) {
        pause = TRUE;
        state = 1;
        update_state();
        select_color = 1;
        update_color();
        return TRUE;
    }
    return FALSE;
}

int validate_and_run_cmd_resume() {
    if (command[0] == 'r' && command[1] == 'e' && command[2] == 's' && command[3] == 'u' && command[4] == 'm' && command[5] == 'e' && command[6] == NULL_CHAR && reset_flag == 0) {
        if (pause == TRUE)
        {
            pause = FALSE;
            select_color = 0;
            state = 0;
            update_state();
            update_color();
            return TRUE;
        }
    }
    return FALSE;
}

int validate_and_run_cmd_restart(){
    if (command[0] == 's' && command[1] == 't' && command[2] == 'o' && command[3] == 'p' && command[4] == NULL_CHAR) {
        if(reset_flag==0) // First press
        {
            time = 0;
            state = 2;
            update_state();
            reset_flag++;
            select_color = 3;
            update_color();
            pause = TRUE;
            return TRUE;
        }
    } else if (command[0] == 's' && command[1] == 't' && command[2] == 'a' && command[3] == 'r' && command[4] == 't' && command[5] == NULL_CHAR) {
        if(reset_flag) // Second press
        {
            state = 0;
            update_state();
            pause = FALSE;
            reset_flag = 0;
            select_color = 0;
            update_color();
            return TRUE;
        }
    }
    return FALSE;
}

int validate_and_run_cmd_tictactoe(){
    if (command[0] == 't' && command[1] == 'i' && command[2] == 'c' && command[3] == 't' && command[4] == 'a' && command[5] == 'c' && command[6] == 't' && command[7] == 'o' && command[8] == 'e' && command[9] == NULL_CHAR) {
        UARTPrint("\n\rStarting Tic-Tac-Toe game.\n\r");
        initialize_tictactoe();
        print_tictactoe();
        TicTacToe_game = TRUE;
        return TRUE;
    }
    return FALSE;
}

void initialize_tictactoe(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            TicTacToe[i][j] = 0;    // 0 = nothing, -1 = 'O', 1 = 'X'
        }
    }
    TicTacToe_winner = 0;
}

void print_tictactoe(){
    UARTPrint("\n\r==================\n\r");
    for(int i=0; i<3; i++){
        UARTPrint("   ");
        for(int j=0; j<3; j++){
            if(TicTacToe[i][j] == 0){
                UARTPrint("   ");
            }else if(TicTacToe[i][j] == 1){
                UARTPrint(" X ");
            }else{
                UARTPrint(" O ");
            }
            if(j!=2){
                UARTPrint("|");
            }
        }
        UARTPrint("\n\r");
        if(i!=2){
            UARTPrint("   -----------\n\r");
        }
    }
    UARTPrint("==================\n\r");
}

void check_winner_tictactoe(){
    
    
    for(int i=0; i<3; i++){
        if((TicTacToe[i][0] == TicTacToe[i][1]) && (TicTacToe[i][1] == TicTacToe[i][2])){
            if(TicTacToe[i][0] != 0){
                TicTacToe_winner = TicTacToe[i][0];
            }
        }
        if((TicTacToe[0][i] == TicTacToe[1][i]) && (TicTacToe[1][i] == TicTacToe[2][i])){
            if(TicTacToe[0][i] != 0){
                TicTacToe_winner = TicTacToe[i][0];
            }
        }
    }
    if((TicTacToe[0][0] == TicTacToe[1][1]) && (TicTacToe[1][1] == TicTacToe[2][2])){
        TicTacToe_winner = TicTacToe[0][0];
    }
    if((TicTacToe[0][2] == TicTacToe[1][1]) && (TicTacToe[1][1] == TicTacToe[2][0])){
        TicTacToe_winner = TicTacToe[0][2];
    }

    if(TicTacToe_winner!=0){
        TicTacToe_game = FALSE;
        TicTacToe_moves = 0;
        if(TicTacToe_winner == 1){
            UARTPrint("\n\r Player 1 (X) wins!\n\r");
        }else{
            UARTPrint("\n\r Player 2 (O) wins!\n\r");
        }
    }else if(TicTacToe_moves == 9){
        TicTacToe_game = FALSE;
        UARTPrint("\n\r It's a draw!\n\r");
    }
}

void run_command() {
    int validate=0;
    switch(command[0])
    {
    case 'p' : {    // pause 
        validate = validate_and_run_cmd_pause();
        clearBuffer();
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
    case 't' : {    // tictactoe
            validate = validate_and_run_cmd_tictactoe();
            clearBuffer();
        }
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
        default: return DISPLAY_NUM_0;
    }
}

void display_time(){
    time = time>10000?0:time;
    unsigned long long displayed_time = time/100;
    unsigned long long digit1 = displayed_time%10;
    unsigned long long digit2 = (displayed_time/10)%10;
    // unsigned long long digit3 = (displayed_time/100)%10; 

    switch (LED_digit)
    {
        case 1:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit1);
            GPIO_PORTA_DATA_R = DIGIT_1;
        }
            break;
        case 2:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit2);
            GPIO_PORTA_DATA_R = DIGIT_2;
        }
        //     break;
        // case 3:{
        //     GPIO_PORTB_DATA_R = int_to_display_num(digit3);
        //     GPIO_PORTA_DATA_R = DIGIT_3;
        // }
    }
    
}

void SysTick_Handler(void){

    if(!pause){
        time++;
    }

    blink_time = (blink_time == blink_delay)?0:blink_time+1;
    
    if(blink_time == blink_delay){
        LED_blink = !LED_blink;
        blink_time = 0;
    } else {
        blink_time++;
    }

    if(LED_digit_delay == SEGMENT_DELAY){
        LED_digit_delay = 0;
        LED_digit = (LED_digit==2)?1:LED_digit+1;
    }else{
        LED_digit_delay++;
    }
}

void GPIOPortF_Handler(void){
    GPIO_PORTF_ICR_R = 0x11;        /* clear PF4 int */

    if(!SW1)   // pause/resume
    {
        if(!reset_flag)
        {
            pause = !pause;
            select_color = pause?1:0;
            state = pause?1:0;
            update_state();
            update_color();
        }
    }

    if(!SW2)   // reset/play
    {
        if(reset_flag==0) // First press
        {
            state = 2;
            update_state();
            time = 0;
            reset_flag++;
            pause = TRUE;
            select_color = 3;
            update_color();
        }else{          // Second press
            pause = FALSE;
            reset_flag = 0;
            select_color = 0;
            state = 0;
            update_state();
            update_color();
        }
    }
}

void update_state(){
    switch(state){
        case 0: strcpy(state_str, "Timer Running");
            break;
        case 1: strcpy(state_str, "Timer Paused");
            break;
        case 2: strcpy(state_str, "Timer Stopped");
            break;
    }
    LCD_display();
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
    State_LCD(state_str);
}

void State_LCD(char*a){

   while(*a != '\0'){
        LCD_command(DATA_RS_1,*a);
        ++a;
    }

}

/* global enable interrupts */
void inline EnableInterrupts(void)
{
    __asm  ("    CPSIE  I\n");
}

void DisableInterrupts(void)
{
    __asm ("    CPSID  I\n");
}
