#include "msp430.h"
#include "LCD_Utilities.h" 
static int Asec=0;
static int sec=0;
static int minute=0;

void main(){
  WDTCTL = WDTPW | WDTHOLD;			
  PM5CTL0 &= ~LOCKLPM5;	
  LCDInit ();
LCD_All_Off();
  P1DIR |= BIT0;
  P1REN|=0x06;
  P1OUT=0x06;
  P1IES |= BIT1|BIT2; 			  
  P1IE  |= BIT1|BIT2;			
  P1IFG = 0x00;	
  TA0CTL = TASSEL__ACLK +  MC__UP    + TACLR         + ID__1       + TAIE;
  TA0CCR0 = 32768/100;
  for(int i=1;i<7;i++) Display_digit(i,0);
  LCDM7=0x04;
  LCDM20=0x04;
  _BIS_SR (LPM3_bits + GIE); 
}
#pragma vector=TIMER0_A1_VECTOR
  __interrupt void Timer (void) {
    if (TA0IV == 0x0E) 
  {
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
    if(minute==99){
      LCD_All_Off();
      Display_letter(6,"THEEND");
    sec=0;
    minute=0;
    Asec=0;  
  for(int i=1;i<7;i++) Display_digit(i,0);
  LCDM7=0x04;
  LCDM20=0x04;
    }
}
  }
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 () 
{
  for (int i = 500;i>0;i--);
  if ((P1IFG &BIT1) && !(P1IN & BIT1) )
  {	
    sec=0;
    minute=0;
    Asec=0;  
   for(int i=1;i<7;i++) Display_digit(i,0);
   TA0CTL|=TAIE;
   P1IFG &= ~BIT1;	
}
if ((P1IFG &BIT2) && !(P1IN & BIT2))
  {	
        TA0CTL&=~TAIE;
        P1IFG &= ~BIT2;
}
}