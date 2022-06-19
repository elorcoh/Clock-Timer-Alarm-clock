#include "msp430.h"
#include "LCD_Utilities.h"
static int i = 1;
static int Asec=0;
static int sec=0;
static int minute=0;


void updateLcd(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
    LCDInit ();                             // initiate LCD
    TA1CTL = TASSEL__ACLK + MC__UP + TACLR+ ID__1 + TAIE;                          	
    TA0CTL = TASSEL__ACLK + MC__UP + TACLR+TAIE;	//  ACLK, UPMODE		
    TA0CCR0 = 16384;                                   //half a second period
    TA1CCR0=32768;
    
   //UART
    P3SEL0 |= BIT4 | BIT5;                    	// USCI_A1  UART operation
    P3SEL1 &= ~(BIT4 | BIT5);
    P3DIR |= BIT4;  
    UCA1CTLW0 = UCSWRST;                 	// Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;    	// CLK = SMCLK
    UCA1BRW = 6;                              	// 9600 baud
    UCA1MCTLW = 0x22D1; 		// UCBRSx value = 0x22 (See UG)
    UCA1CTL1 &= ~UCSWRST;                	// Initialize eUSCI
    UCA1IE|=UCA1IE_;
    while(!(UCA1IFG&UCRXIFG)); 



    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();

    return 0;
}
/*
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR() {
  if  (UCA1IFG & UCRXIFG)  {
	UCA1IFG &= ~ UCRXIFG;  
        if(UCA1RXBUF==1){
          
      
        
          
  }	
}

*/
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
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer(void) {
	if (TA0IV == 0x0E) {
         
           LCDMEM[6] = 0x04;
           LCDMEM[19] = 0x04;
           if (i == 2){
	   LCDMEM[6] = 0x0;
           LCDMEM[19] = 0x0;
           i=0;}
           i++;
   
        }
	
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer(void) {
  if (TA1IV==0x0E){
       Display_digit(6,Asec%10);
      Display_digit(5,Asec/10);
    Asec++;
    if(Asec==100){
       sec++;
      Asec=0;
      Display_digit(4,sec%10);
      Display_digit(3,sec/10);
    }
    if(sec==60){
      sec=0;
      Asec=0;
      minute++;
       Display_digit(2,minute%10);
      Display_digit(1,minute/10);
    }
    
  }}
void updateLcd(void)
{
    Display_digit(1,(RTCHOUR & 0xF0) >> 4);
    Display_digit(2,(RTCHOUR & 0x0F));
    Display_digit(3,(RTCMIN & 0xF0) >> 4);
    Display_digit(4,(RTCMIN & 0x0F));
    Display_digit(5,(RTCSEC & 0xF0) >> 4);
    Display_digit(6,(RTCSEC & 0x0F));
}





