#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include<stdio.h>
 
//********************Macro for Pins and Clock**************
#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
 
//**********************Macro for Colors********************

#define COLOR_GREEN_ON  0x08
#define COLOR_BLUE_ON   0x04
#define COLOR_CYAN_ON   0x0C
#define COLOR_RED_ON    0x02
#define COLOR_YELLOW_ON 0x0A
#define COLOR_MAGENTA_ON 0x06
#define COLOR_WHITE_ON  0x0E

#define MAX_BLINK_DELAY 2000
#define MIN_BLINK_DELAY 100
#define DEBOUNCING_DELAY DEBOUNCING_DELAY

#define SW1 ((GPIO_PORTF_DATA_R & (1<<4))>>4)
#define SW2 ((GPIO_PORTF_DATA_R & (1<<0))>>0)

uint8_t delay;
int blink_delay = MAX_BLINK_DELAY;
int select_color = 0;
uint8_t color = COLOR_GREEN_ON;
int flag = 0;
 
int main ()
{
 
    SYSCTL_RCGC2_R |= 0x00000020;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlockGPIOCR register
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_DEN_R |= PIN0 + PIN1 + PIN2 + PIN3 + PIN4; 
    GPIO_PORTF_DIR_R |= PIN1 + PIN2 + PIN3; 
    GPIO_PORTF_DIR_R &= (~PIN4); // Set PF4 (SW1) as input
    GPIO_PORTF_DIR_R &= (~PIN0); // Set PF0 (SW2) as input
    GPIO_PORTF_PUR_R |= PIN4 + PIN0; // Enable pull-up resistor for PF4 (SW1) and PF0 (SW2)
 
while(1)
{
    if (flag==0){
        GPIO_PORTF_DATA_R = color;
        delayMs_check(blink_delay);
        GPIO_PORTF_DATA_R = 0x00;
        delayMs_check(blink_delay);
    }
 
    if(SW1 == 0)
    {
        select_color = (select_color + 1)%7;
        update_color();
        delayMs(DEBOUNCING_DELAY);
    }
 
    if(SW2 == 0)
        {
                if(blink_delay > MIN_BLINK_DELAY)
                {
                    blink_delay = blink_delay/2;
                    flag=0;
                    delayMs(DEBOUNCING_DELAY);
                }
                else
                {
                    blink_delay = MAX_BLINK_DELAY;
                    GPIO_PORTF_DATA_R = color;
                    flag=1;
                    delayMs(DEBOUNCING_DELAY);
                }
        }
}
}
 
void update_color(){
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
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
 
void delayMs_check(int n)
{
int i, j;
    for(i = 0 ; i < n; i++)
    {
 
       if(SW1 == 0)
       {
            select_color = (select_color + 1)%7;
            update_color();
            delayMs(DEBOUNCING_DELAY);
            break;
       }
 
        if(SW2== 0)
        {
                if(blink_delay > MIN_BLINK_DELAY)
            {
                blink_delay = blink_delay/2;
                flag=0;
                delayMs(DEBOUNCING_DELAY);
            }
            else
            {
                blink_delay = MAX_BLINK_DELAY;
                GPIO_PORTF_DATA_R = color;
                flag=1;
                delayMs(DEBOUNCING_DELAY);
            }
            delayMs(DEBOUNCING_DELAY);
            break;
        }

        for(j = 0; j < 3180; j++){} /* do nothing for 1 ms */
    }
}