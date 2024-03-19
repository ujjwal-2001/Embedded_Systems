// UJJWAL CHAUDHARY, 22577, M. Tech. ESE, IISc Bengaluru
// LAB 07

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

//GLOBAL VARIABLES

uint8_t delay;
int pwm_value = 4999;
long blink_delay = TYPICAL_BLINK_DELAY;
int blink_time = 0;
int select_color = 0;
int color_change_count = 0;
int LED_blink = 0;
int pause = FALSE;
int reset_flag = 0;
uint8_t color = COLOR_GREEN_ON;
char command[MAX_BUFFER_SIZE]={NULL_CHAR};
int cmd_char_index = 0;
unsigned long long time = 1000000;
int LED_digit = 1;
int LED_digit_delay = 0;
int display_pwm_flag = 0;
int pwm_display_timer = 0;

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
void check_winner_tictactoe();
void run_command();
void correct_cmd();
int int_to_display_num(int num);
void display_time();
void check_command();
void display_PWM();

//MAIN

int main ()
{
    Initialize();
    UARTPrint("\n\rWelcome to the Stop Watch System.\n\r");
    print_commands();
    UARTPrint("\n\r");
    float temp;
    int temp2;
    volatile int result;    
    while(1)
    {
        
        if(LED_blink)
            GPIO_PORTF_DATA_R = color;
        else
            GPIO_PORTF_DATA_R = NO_COLOR;
        
        if(display_pwm_flag)
        {
            display_PWM();
        }
        else
        {
            display_time();
        }

        check_input();

        ADC0_PSSI_R |= 8;               /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0);   /* wait for conversion complete */
        result = ADC0_SSFIFO3_R;        /* read conversion result */
        ADC0_ISC_R = 8;                 /* clear completion flag */
        temp=(((result)/4095.0)*4999.0);
        temp2=(int)temp;
        pwm_value = 4999-temp2;
        PWM1_3_CMPA_R= pwm_value;
        long temp_blink_delay = (MAX_BLINK_DELAY* pwm_value)/4999; // x value is withing [4999-0]
        
        if((temp_blink_delay-blink_delay >= 30) || temp_blink_delay-blink_delay <= -30){
            display_pwm_flag = 1;
        }

        blink_delay = temp_blink_delay;
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

    // Configure PWM
    
    /* Enable Peripheral Clocks */
    SYSCTL_RCGCPWM_R |= 2; /* enable clock to PWM Module 1 */
    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PORTE */
    SYSCTL_RCC_R &= ~0x00100000; /* no pre-divide for PWM clock */
    SYSCTL_RCC_R |= 0x000E0000;
    /* Enable port PE4 for PWM1 M1PWM2 */
    GPIO_PORTE_AFSEL_R = 0x10; /* PE4 uses alternate function */
    GPIO_PORTE_PCTL_R &= ~0x000F0000; /* make PF2 PWM output pin */
    GPIO_PORTE_PCTL_R |= 0x00050000;
    GPIO_PORTE_DEN_R |= 0x10; /* pin digital */
 
    //PWM Module 1 Generator 1
    PWM1_1_CTL_R &= ~(1<<0); /* stop counter */
    PWM1_1_CTL_R &= ~(1<<1);
    PWM1_1_GENA_R = 0x0000008C; /* M1PWM6 output set when reload, */
    /* clear when match PWMCMPA */
    PWM1_1_LOAD_R = 5000; /* set load value for 1kHz (16MHz/16000) */
    PWM1_1_CMPA_R = 4999; /* set duty cycle to min */
    PWM1_1_CTL_R = 1; /* start timer */
    PWM1_ENABLE_R = 0x04; /* start PWM1 ch2 */
 
    SYSCTL_RCGCGPIO_R |= 0x04;
 
    GPIO_PORTC_DIR_R |= (1<<5)|(1<<4);
    GPIO_PORTC_DEN_R |= (1<<5)|(1<<4);
    GPIO_PORTC_DATA_R |= (1<<5);
    GPIO_PORTC_DATA_R &= ~(1<<4);
 
    SYSCTL_RCGCADC_R |= 0x00000001; /* enable clock to ADC0 */
    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PE (AIN0 is on PE3) */
 
    ADC0_ACTSS_R |= 0x00000008; /* enable ADC0 sequencer 3 */
    ADC0_EMUX_R &= ~0xF000; /* software trigger conversion */
    ADC0_SSMUX3_R = 0; /* get input from channel 0 */
    /* initialize PE3 for AIN0 input */
    GPIO_PORTE_AFSEL_R |= 8; /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8; /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8; /* enable analog function */
    ADC0_SSCTL3_R |= 6; /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 8; /* enable ADC0 sequencer 3 */
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
        \n\r");
}

int validate_and_run_cmd_pause() {
    if (command[0] == 'p' && command[1] == 'a' && command[2] == 'u' && command[3] == 's' && command[4] == 'e' && command[5] == NULL_CHAR && reset_flag == 0) {
        pause = TRUE;
        
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
            reset_flag++;
            select_color = 3;
            update_color();
            pause = TRUE;
            return TRUE;
        }
    } else if (command[0] == 's' && command[1] == 't' && command[2] == 'a' && command[3] == 'r' && command[4] == 't' && command[5] == NULL_CHAR) {
        if(reset_flag) // Second press
        {
            pause = FALSE;
            reset_flag = 0;
            select_color = 0;
            update_color();
            return TRUE;
        }
    }
    return FALSE;
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
        } break;
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
    time = time==0?1000000:time;
    unsigned long long displayed_time = time/100;
    int digit1 = displayed_time%10;
    int digit2 = (displayed_time/10)%10;
    int digit3 = (displayed_time/100)%10;
    int digit4 = (displayed_time/1000)%10; 

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
            break;
        case 3:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit3);
            GPIO_PORTA_DATA_R = DIGIT_3;
        } break;
        case 4:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit4);
            GPIO_PORTA_DATA_R = DIGIT_4;
        } break;
    }
    
}

void display_PWM(){
    
    int digit1 = pwm_value%10;
    int digit2 = (pwm_value/10)%10;
    int digit3 = (pwm_value/100)%10;
    int digit4 = (pwm_value/1000)%10; 

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
            break;
        case 3:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit3);
            GPIO_PORTA_DATA_R = DIGIT_3;
        } break;
        case 4:{
            GPIO_PORTB_DATA_R = int_to_display_num(digit4);
            GPIO_PORTA_DATA_R = DIGIT_4;
        } break;
    }
}

void SysTick_Handler(void){

    if(!pause){
        time--;
    }
    
    if(blink_time >= blink_delay){
        LED_blink = !LED_blink;
        blink_time = 0;
    } else {
        blink_time++;
    }

    if(LED_digit_delay == SEGMENT_DELAY){
        LED_digit_delay = 0;
        LED_digit = (LED_digit==4)?1:LED_digit+1;
    }else{
        LED_digit_delay++;
    }

    if(display_pwm_flag){
        if(pwm_display_timer >= 2000){
            display_pwm_flag = 0;
            pwm_display_timer = 0;
        }else{
            pwm_display_timer++;
        }
    
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
            update_color();
        }
    }

    if(!SW2)   // reset/play
    {
        if(reset_flag==0) // First press
        {
            time = 0;
            reset_flag++;
            pause = TRUE;
            select_color = 3;
            update_color();
        }else{          // Second press
            pause = FALSE;
            reset_flag = 0;
            select_color = 0;
            update_color();
        }
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++){
        for(j = 0; j < 3180; j++);
    } /* do nothing for 1 ms */
}


/* global enable interrupts */
void static inline EnableInterrupts(void)
{
    __asm  ("    CPSIE  I\n");
}

void static inline DisableInterrupts(void)
{
    __asm ("    CPSID  I\n");
}
