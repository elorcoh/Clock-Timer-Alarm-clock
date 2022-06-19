#include "msp430.h"
#include "LCD_Utilities.h" 
static int Asec=0;
static int sec=0;
static int minute=0;
static int mode=3;
static int submode=1;
void main(){ 
  WDTCTL = WDTPW | WDTHOLD;			
  PM5CTL0 &= ~LOCKLPM5;	
  LCDInit ();
LCD_All_Off();
//stopper//
 TA3CTL = TASSEL__ACLK +  MC__UP    + TACLR         + ID__1       + TAIE;
  TA3CCR0 = 32768/100;
 for(int i=1;i<7;i++) Display_digit(i,0);
  LCDM7=0x04;
  LCDM20=0x04;
  _BIS_SR (LPM3_bits + GIE); 
}
#pragma vector=TIMER3_A1_VECTOR
  __interrupt void TimerA3 (void) {
    if (TA3IV == 0x0E) 
	{        
                    
          
		if (mode == 3 && submode==0)
		{
		 for(int i=1;i<7;i++) Display_digit(i,0);
                    

	}
        if (mode == 3 && submode == 1) {
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
      Display_letter(6,"PIZDEZ");
    sec=0;
    minute=0;
    Asec=0;  
  for(int i=1;i<7;i++) Display_digit(i,0);
  LCDM7=0x04;
  LCDM20=0x04;
    }
}
  }

        
		if (mode == 3 && submode == 2) {
			TA3CTL &= ~TAIE;
		}
		

  
    }

}
  
