
#include "msp430.h"
#include "LCD_Utilities.h"
static int i = 1 ,j=1,x=1;

void blinkHour(void);
void blinkMin(void);
void updateLcd(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
    LCDInit ();                             // initiate LCD
    
    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC
    RTCCTL0_L = RTCTEVIE | RTCRDYIE;        // enable RTC read ready interrupt
                                            // enable RTC time event interrupt

    RTCCTL1 = RTCBCD | RTCHOLD | RTCMODE;   // RTC enable, BCD mode, RTC hold

    RTCYEAR = 0x2010;                       // Year = 0x2010
    RTCMON = 0x4;                           // Month = 0x04 = April
    RTCDAY = 0x05;                          // Day = 0x05 = 5th
    RTCDOW = 0x01;                          // Day of week = 0x01 = Monday
    RTCHOUR = 0x04;                         // Hour = 0x04
    RTCMIN = 0x30;                          // Minute = 0x30
    RTCSEC = 0x45;                          // Seconds = 0x45
    RTCCTL1 &= ~(RTCHOLD);                  // Start RTC

    updateLcd();                             // Display on LCD	
    TA2CTL = TASSEL__ACLK + MC__UP + TACLR+TAIE;	//  ACLK, UPMODE		
    TA2CCR0 = 16384;                                   //half a second period
 
   //UART
    P3SEL0 |= BIT4 | BIT5;                    	// USCI_A1  UART operation
    P3SEL1 &= ~(BIT4 | BIT5);
    P3DIR |= BIT4;  
    UCA1CTLW0 = UCSWRST;                 	// Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;    	// CLK = SMCLK
    UCA1BRW = 6;                              	// 9600 baud
    UCA1MCTLW = 0x22D1; 		// UCBRSx value = 0x22 (See UG)
    UCA1CTL1 &= ~UCSWRST;                	// Initialize eUSCI
   // UCA1IE|=UCA1IE_;
    



    __bis_SR_register(LPM3_bits | GIE);
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
            updateLcd();                    // Update the segmented LCD
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
#pragma vector=TIMER2_A1_VECTOR
__interrupt void Timer(void) {
	if (TA2IV == 0x0E) {
         if(mode==1 && submode==0)blinkHour();
          else if(mode==1 && submode==1)blinkMin();
         if(mode==2 && submode==1) blinkHour();
         else if (mode==2 && submode==2) blinkMin();
         else{
           LCDMEM[6] = 0x04;
           LCDMEM[19] = 0x04;
           if (i == 2){
	   LCDMEM[6] = 0x0;
           LCDMEM[19] = 0x0;
           i=0;}
           i++;
         }
        }
	 
       
}



void updateLcd(void)
{
    Display_digit(1,(RTCHOUR & 0xF0) >> 4);
    Display_digit(2,(RTCHOUR & 0x0F));
    Display_digit(3,(RTCMIN & 0xF0) >> 4);
    Display_digit(4,(RTCMIN & 0x0F));
    Display_digit(5,(RTCSEC & 0xF0) >> 4);
    Display_digit(6,(RTCSEC & 0x0F));
}

void blinkHour(void)
{
  
  if(j==1){
LCDM10 &=0X1;
  LCDM11 &=0X1;
  LCDM6 &=0X1;
  LCDM7 &=0X1;
 }
  if(j==2){
  LCDM10 &=0X0;
  LCDM11 &=0X0;
  LCDM6 &=0X0;
  LCDM7 &=0X0;
  j=0;}
  j++;
  
}
void blinkMin(void)
{
    if(j==1){
LCDM5 &=0X1;
  LCDM4 &=0X1;
  LCDM19 &=0X1;
  LCDM20 &=0X1;
 }
  if(j==2){
  LCDM5 &=0X0;
  LCDM4 &=0X0;
  LCDM19 &=0X0;
  LCDM20 &=0X0;
  j=0;}
  j++;
  
}
  
  