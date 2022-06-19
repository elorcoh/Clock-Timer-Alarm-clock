
#include <msp430.h>
#include "LCD_Utilities.h"

int main(void)
{
  void LCDInit ();
void LCD_All_On();
void LCD_All_Off();
    WDTCTL = WDTPW | WDTHOLD;               // Stop Watchdog Timer

    P1DIR |= BIT0;                          // Set P1.0 as output

    PJSEL0 = BIT4 | BIT5;                   // Initialize LFXT pins

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC
    RTCCTL0_L = RTCTEVIE | RTCRDYIE;        // enable RTC read ready interrupt
                                            // enable RTC time event interrupt

    RTCCTL1 = RTCBCD | RTCHOLD | RTCMODE;   // RTC enable, BCD mode, RTC hold

    RTCYEAR = 0x2010;                       // Year = 0x2010
    RTCMON = 0x4;                           // Month = 0x04 = April
    RTCDAY = 0x05;                          // Day = 0x05 = 5th
    RTCDOW = 0x01;                          // Day of week = 0x01 = Monday
    RTCHOUR = 0x10;                         // Hour = 0x10
    RTCMIN = 0x32;                          // Minute = 0x32
    RTCSEC = 0x45;                          // Seconds = 0x45

    RTCADOWDAY = 0x2;                       // RTC Day of week alarm = 0x2
    RTCADAY = 0x20;                         // RTC Day Alarm = 0x20
    RTCAHOUR = 0x10;                        // RTC Hour Alarm
    RTCAMIN = 0x23;                         // RTC Minute Alarm

    RTCCTL1 &= ~(RTCHOLD);                  // Start RTC

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3 mode w/ interrupts enabled
    __no_operation();

    return 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(RTCIV, RTCIV_RT1PSIFG))
    {
        case RTCIV_NONE:      break;        // No interrupts
        case RTCIV_RTCOFIFG:  break;        // RTCOFIFG
        case RTCIV_RTCRDYIFG:               // RTCRDYIFG
            P1OUT ^= 0x01;                  // Toggles P1.0 every second
            Display_digit(
            break;
        case RTCIV_RTCTEVIFG:               // RTCEVIFG
            __no_operation();               // Interrupts every minute
            break;
        case RTCIV_RTCAIFG:   break;        // RTCAIFG
        case RTCIV_RT0PSIFG:  break;        // RT0PSIFG
        case RTCIV_RT1PSIFG:  break;        // RT1PSIFG
        default: break;
    }
}
