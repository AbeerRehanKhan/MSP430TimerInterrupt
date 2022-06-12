#include <msp430.h>
#include <stdio.h>
#include "lcdLib.h"
//Change this as per your needs

unsigned int OFCount;
int timervalue = 500;

#define BUTTON BIT3
#define LED1 BIT0
#define LED2 BIT6

int p_count = 1;
float a_freq = 10;
int count = 40;
char timercase = 'I';

float a_mult[20] = {0.4,0.45,0.5,0.55,0.6,0.7,0.8,0.9,0.95,1,1.05,1.1,1.3,1.7,1.9,1.95,2,2.05,2.1,2.3};
float lf_range[3] = {1,5,20}  ;
float hf_range[3] = {11,8,21} ;
float f_range;


void initTimer_A(void);
void delayMS(int msecs);

int main(void)
z {
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
    P1DIR |= BIT0; //Configure P1.0 as Output

    //Set MCLK = SMCLK = 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    lcdInit();
    int r = 0;
    for(r = 0; r < 3 ; r++){
        f_range  = lf_range[r];
        while(f_range< hf_range[r]){
            timervalue = 5000/(f_range*10);


            initTimer_A();
            _enable_interrupt();

            OFCount  = 0;
            TACCR0 = 1000-1; //Start Timer, Compare value for Up Mode to get 1ms delay per loop
            /*Total count = TACCR0 + 1. Hence we need to subtract 1.
            1000 ticks @ 1MHz will yield a delay of 1ms.*/
            while(count < 60);
            count = 0;
            timercase = 'O';
            timervalue = 1000 - 1;
            while(count < 5);
            count = 0;
            _disable_interrupt();
            f_range +=0.1;
            timercase = 'I';


        }
            }
    }

void initTimer_A(void)
{
    //Timer Configuration
    TACCR0 = 0; //Initially, Stop the Timer
    TACCTL0 |= CCIE; //Enable interrupt for CCR0.
    TACTL = TASSEL_2 + ID_0 + MC_1; //Select SMCLK, SMCLK/1 , Up Mode
}

//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    OFCount++;
    if(OFCount >= timervalue & count < 60 & timercase == 'I')
    {
        lcdSetText("Freq:", 0, 0);
        lcdSetInt(f_range*100, 9, 0);
        lcdSetText("Count:", 0,1);
        lcdSetInt(count, 9, 1);

        P1OUT ^= 0x01;              // toggle P1.0
        P1OUT ^= BIT1;              // toggle P1.1
        OFCount = 0;
        count++;
    }
    else if(OFCount >= timervalue & timercase == 'O')
    {
        lcdSetText("Pause", 0, 0);
        lcdSetText("for 5 sec", 0, 0);
        P1OUT = 0x00;
        OFCount = 0;
        count++;

    }
}
