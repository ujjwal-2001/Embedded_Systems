#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>


int timearray[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int redled =  0x02;
int blueled =  0x04;
int greenled =  0x08;
int offled =  0x00;

int segment_count = 0;
int ledcount = 0;
int timer_count = 0;
int update_timer = 0;

int countsec1 = 0,countsec2 = 0,countmin1 = 0,countmin2 = 0;
int switch_status;
int currstate = 2;      // 1 - Running, 2 - Stop, 3 - Pause

void runningstate(void);
void stopstate(void);
void pausestate(void);
void timeroperation(void);
void EnableInterrupts(void);
void initialize_ports(void);
void DisableInterrupts(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

int main(void)

{

initialize_ports();
EnableInterrupts();                   /* global enable interrupt */

char val;
char cmd_str[128] = "";

while(1) {

    if (UARTCharsAvail(UART0_BASE))
    {
        val = UARTCharGet(UART0_BASE);
        UARTCharPut(UART0_BASE, val);       // UART echoback

        if (val == '\n'||val == '\r')
        {
            if (strcasecmp(cmd_str,"sw start")==0)
                {

                if(currstate == 3){
                    UARTSend((uint8_t *)"Err:IN HOLD", 12);
                }
                else{
                    currstate = 1;
                    countsec1 = 0,countsec2 = 0,countmin1 = 0,countmin2 = 0;

                }

                }
            else if (strcasecmp(cmd_str,"sw stop")==0)
                {
                currstate = 2;
                }

            else if (strcasecmp(cmd_str,"sw hold")==0)
                {
                if(currstate == 2){
                    UARTSend((uint8_t *)"Err:IN STOP", 12);
                }
                else{
                    currstate = 3;
                }
                }
            else if (strcasecmp(cmd_str,"sw resume")==0)
                {
                if(currstate == 2){
                    UARTSend((uint8_t *)"Err:IN STOP", 12);
                }
                else{
                    currstate = 1;
                }
                }
            cmd_str[0] = '\0';
            UARTCharPut(UART0_BASE, '\n');
        }
        else
            strncat(cmd_str, &val, 1);
    }

    if (currstate == 1){
        runningstate();
       if ((ledcount > 900) && (switch_status/10 == 1)){
         currstate = 2;
         switch_status = 0;
         ledcount = 0;
         timer_count = 0;

       }
       else if ((ledcount > 900) && (switch_status % 10 == 1)){
           currstate = 3;
           timer_count = 0;
           switch_status = 0;
           ledcount = 0;
         }

    }

    else if (currstate == 2){
        stopstate();
       if ((ledcount > 900) && (switch_status/10 == 1)){
         countsec1 = 0,countsec2 = 0,countmin1 = 0,countmin2 = 0;
         currstate = 1;
         ledcount = 0;
         timer_count = 0;
         switch_status = 0;

       }
    }

    else if (currstate == 3){
       pausestate();
       if ((ledcount > 900) && (switch_status/10 == 1)){
         currstate = 2;
         ledcount = 0;
         timer_count = 0;
         switch_status = 0;

       }
       else if ((ledcount > 900) && (switch_status % 10 == 1)){
           currstate = 1;
           ledcount = 0;
           timer_count = 0;
           switch_status = 0;


         }
    }

}

}


void runningstate(void)
{

    if ((ledcount <= 250) || ((ledcount <= 750) && (ledcount > 500) )){
        GPIO_PORTF_DATA_R = greenled;
    }
    else{
        GPIO_PORTF_DATA_R = offled;
    }

    timeroperation();

  if (update_timer == 1){

    if ((countmin1 == 9) && (countsec2 == 5) && (countsec1 == 9) ){
        if (countmin2 == 9){
            countmin2 = 0;
        }
        else{
           countmin2 = countmin2 + 1;
        }
    }

    if ((countsec2 == 5) && (countsec1 == 9) ){
        if (countmin1 == 9){
            countmin1 = 0;
        }
        else{
           countmin1 = countmin1 + 1;
        }

    }

    if ((countsec1 == 9) ){

        if (countsec2 == 5){
            countsec2 = 0;
        }
        else{
        countsec2 = countsec2 + 1;
        }
        countsec1 = 0;
    }
    else{
        countsec1 = countsec1 + 1;
    }
    update_timer = 0;
  }

}

void stopstate(void){

    GPIO_PORTF_DATA_R = redled;
    timeroperation();

}

void pausestate(void){


    if ((ledcount <= 250) || ((ledcount <= 750) && (ledcount > 500) )){
        GPIO_PORTF_DATA_R = blueled;
    }
    else{
        GPIO_PORTF_DATA_R = offled;
    }

    timeroperation();

}


void timeroperation(void){

    if (segment_count == 0){
        GPIO_PORTA_DATA_R = 0x40;
        GPIO_PORTB_DATA_R = 0x80;
  }
    else if(segment_count == 1){
        GPIO_PORTA_DATA_R = 0x10;
        GPIO_PORTB_DATA_R = timearray[countsec1];
    }
    else if(segment_count == 2){
        GPIO_PORTA_DATA_R = 0x20;
        GPIO_PORTB_DATA_R = timearray[countsec2];
    }
    else if(segment_count == 3){
        GPIO_PORTA_DATA_R = 0x40;
        GPIO_PORTB_DATA_R = timearray[countmin1];
    }
    else if(segment_count == 4){
        GPIO_PORTA_DATA_R = 0x80;
        GPIO_PORTB_DATA_R = timearray[countmin2];
    }

}

void initialize_ports(void){

    SYSCTL_RCGC2_R |= 0x00000023;

    GPIO_PORTB_AMSEL_R = 0x00;
    GPIO_PORTB_PCTL_R = 0x00;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTB_AFSEL_R = 0x00;
    GPIO_PORTB_DEN_R = 0xFF;


    GPIO_PORTA_AMSEL_R = 0x00;
    GPIO_PORTA_PCTL_R = 0x00;
    GPIO_PORTA_DIR_R = 0xFF;
    GPIO_PORTA_AFSEL_R = 0x00;
    GPIO_PORTA_DEN_R = 0xFF;


    GPIO_PORTF_LOCK_R = 0x4C4F434B; /* unlock commit register */
    GPIO_PORTF_CR_R = 0x01;

    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x11; /* enable pull up for pin 4 */

    GPIO_PORTF_IS_R &= ~0x11;       /*  PF is edge-sensitive */
    GPIO_PORTF_IBE_R &= ~0x11;      /*  PF is not both edges */
    GPIO_PORTF_IEV_R &= ~0x11;      /*  PF falling edge event */
    GPIO_PORTF_ICR_R = 0x11;        /*  Clear flag */
    GPIO_PORTF_IM_R |= 0x11;        /*  arm interrupt on PF */
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000; /*  priority 5 for Port F*/
    NVIC_EN0_R = 0x40000000;        /*  Enable interrupt 30 in NVIC */

    NVIC_ST_RELOAD_R = 16000;           //Load timer value
    NVIC_ST_CTRL_R = 7;                 //Enable SYSTICK interrupt
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x0FFFFFFF) | 0x20000000; /*  priority 1 for SYSTICK*/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTCharPut(UART0_BASE, 0x1B);
    UARTCharPut(UART0_BASE, '[');
    UARTCharPut(UART0_BASE, '2');
    UARTCharPut(UART0_BASE, 'J');

}

void SysTick_Handler(void)
{

    if (ledcount < 1000){
         ledcount = ledcount + 1;
     }
     else{
         ledcount = 0;
     }

    if (segment_count < 4){
        segment_count = segment_count + 1;
    }
    else{
        segment_count = 0;
    }

    if (timer_count < 1000){
        timer_count = timer_count + 1;
        update_timer = 0;
    }
    else{
        timer_count = 0;
        update_timer = 1;
    }

}

void GPIOPortF_Handler(void)
{
    volatile int readback;
    int sw1_status = 0,sw2_status = 0;
    switch_status = 0;

    GPIO_PORTF_ICR_R = 0x11;        /* clear PF4 int */
    readback = GPIO_PORTF_ICR_R;    /* a read to force clearing of interrupt flag */
    readback = readback;

    if((GPIO_PORTF_DATA_R & 0x10) == 0x00)
    {
        sw1_status = 1;

    }

    if((GPIO_PORTF_DATA_R & 0x01) == 0x00)
    {
        sw2_status = 1;

    }

    switch_status = sw1_status*10+sw2_status;

}


void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{

    while(ui32Count--)
    {
        UARTCharPut(UART0_BASE, *pui8Buffer++);
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


