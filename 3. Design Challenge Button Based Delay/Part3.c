#include <msp430.h>

char Timer = 0x00;
int Hold = 0;
void gpioInit(); // GPIO initialization
void timerInit(); // Timer initialization



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //stops the watchdog timer

    gpioInit();
    timerInit();

    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    __bis_SR_register(GIE); //LPM3

    while(1)
    {
        if (Timer == 0x00)
        {
            TB1CTL = TBSSEL_1 | MC_2 | ID_1;
        }
        if (Timer == 0x01)
        {
            TB1CTL = TBSSEL_1 | MC_2 | ID_3;
        }
    }
}
void gpioInit()
{

      P1OUT &= ~BIT0;                         // Clear P1.0 output latch
      P1DIR |= BIT0;                          // P1.0 to output

      P2OUT |= BIT3;                          // pull up
      P2REN |= BIT3;                          // pull up enable
      P2IES |= BIT3;
      P2IE |= BIT3;                           // P2.3 interrupt enabled

      P4OUT |= BIT1;                          // pull up
      P4REN |= BIT1;                          // pull up enable
      P4IES |= BIT1;                       m  //
      P4IE |= BIT1;                           // P4.1 interrupt enabled

}
void timerInit()
{
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 4096;                           // start CCR
    TB1CTL = TBSSEL_1 | MC_2 | TBCLR | ID_1 | TBIE;                 // ACLK, cont. mode
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
    if (Timer == 0x00)
    {
        P1OUT ^= BIT0;
        TB1CCR0 += 4096; // Add Offset to TB1CCR0
    }
    else if (Timer == 0x01)
    {
        P1OUT ^= BIT0;
        TB1CCR0 += HoldTime;
    }
}
// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear IFG

    TB1CTL = TBSSEL_1 | MC_2 | TBCLR | ID_3 | TBIE;
    Timer = 0x01;

    while (~P2IN & BIT3)
    {
        Hold = TB1R;
    }

}
// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    // @TODO You might need to modify this based on your approach to the lab
    P4IFG &= ~BIT1;                         // Clear IFG
    TB1CCR0 = 4096;
    Timer == 0x00;

}
