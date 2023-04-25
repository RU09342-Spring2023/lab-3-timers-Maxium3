/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();
char LED = 0x00;
void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

    while(1)
    {
            if(LED==0x00)
            {
                TB1CTL = TBSSEL_1 | MC_2 | ID_2;
            }
            else if(LED==0x01)
            {
                TB1CTL = TBSSEL_1 | MC_2 | ID_1;
            }
            else if(LED==0x02)
            {
                TB1CTL = TBSSEL_1 | MC_2 | ID_0;
            }

        }
}


void gpioInit(){
    // @TODO Initialize the Red or Green LED
    P1DIR |= BIT0;
    // @TODO Initialize Button 2.3
    P2DIR &= ~BIT3;
    P2IN &= ~BIT3;
    P2REN |= BIT3;                                // Enables pull-up resistor on button P2.3


}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

        TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
        TB1CCR0 = 50000;
        TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode

        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3 w/ interrupt
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P2IFG &= ~BIT3;

    if ((P2IES &= BIT3) == BIT3)
        {
            switch (LED)
            {
                case 0x00:
                    LED = 0x01;
                    break;
                case 0x01:
                    LED = 0x02;
                    break;
                case 0x02:
                    LED = 0x00;
                    break;



    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

}


// Timer B1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
    P1OUT ^= BIT0;
    TB1CCR0 += 32768;
}


