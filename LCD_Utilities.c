#include "LCD_Utilities.h"
#include <string.h>
#define NEG 0x04
#define ERR  0x9F
#define SIGN_LESS_ON LCDM11 |= NEG
#define SIGN_LESS_OFF LCDM11 &= ~NEG
#define SIGN_ERR_ON LCDM8 = ERR
// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28} /* "Z" */
};
void LCDInit (){
    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
      LCDCCTL0 |= LCDON;

 }
void wait(int ms) {
	int static i, j;
	for(i=ms; i>0; i--)
	 	for(j=100; j>=0; j--);
}
void Display_digit(int p,int q){
  char lcd_num[10] = {0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE4,  0xFF, 0xF7};
  char *Ptr2Num[6] = {0};
    Ptr2Num[0] +=0xA29;
    Ptr2Num[1] +=0xA25;
    Ptr2Num[2] +=0xA23;
    Ptr2Num[3] +=0xA32;
    Ptr2Num[4] +=0xA2E;
    Ptr2Num[5] +=0xA27;
    *Ptr2Num[p-1]=lcd_num[q];
}

void LCD_All_On(){
int i;
	char *ptr = 0;
	ptr += 0x0A20;		// LCD memory starts at 0x0A20
	for (i = 0;i<21;i++)
		*ptr++ = 0xFF;

}



void LCD_All_Off(){

int i;
	char *ptr = 0;
	ptr += 0x0A20;		// LCD memory starts at 0x0A20
	for (i = 0;i<21;i++)
		*ptr++ = 0x00;

}


void Display_Number (long long n){
  int i=0;
   char lcd_num[10] = {0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE4,  0xFF, 0xF7};
   char *Ptr2Num[6] = {0};
    Ptr2Num[0] +=0xA29;
    Ptr2Num[1] +=0xA25;
    Ptr2Num[2] +=0xA23;
    Ptr2Num[3] +=0xA32;
    Ptr2Num[4] +=0xA2E;
    Ptr2Num[5] +=0xA27;
  LCD_All_Off();
  if(n<0){
    	n=-n;
    	SIGN_LESS_ON;
   } 
  if (n>999999){
    	SIGN_ERR_ON;
    	return;
    }
   do {
        *Ptr2Num[5-i] = lcd_num[n%10];
         i++;
         n = n/10;  // wastefull!!
    }while ( n );
}
void Display_letter (int size,char *letter) {
  char *Ptr2Num[12] = {0};
    Ptr2Num[0] +=0xA29;//LCDM11_A1
    Ptr2Num[1] +=0xA2A;//LCDM10_A1
    Ptr2Num[2] +=0xA25;//LCDM6_A2
    Ptr2Num[3] +=0xA26;//LCDM7_A2
    Ptr2Num[4] +=0xA23;//LCDM4_A3
    Ptr2Num[5] +=0xA24;//LCDM5_A3
    Ptr2Num[6] +=0xA32;//LCDM19_A4
    Ptr2Num[7] +=0xA33;//LCDM20_A4
    Ptr2Num[8] +=0xA2E;//LCDM15_A5
    Ptr2Num[9] +=0xA2F;//LCDM16_A5
    Ptr2Num[10] +=0xA27;//LCDM8_A6
    Ptr2Num[11] +=0xA28;//LCDM9_A6
    for(int i=0;i<size;i++){
    *Ptr2Num[i*2]=alphabetBig[(letter[i]-65)][0];  
    *Ptr2Num[(i*2)+1]=alphabetBig[(letter[i]-65)][1];
}
wait(1000);
}
void shiftright (char *myarray, int size, int shiftBy)
{
    if(shiftBy > size){
        shiftBy = shiftBy - size;
    }
    if(size == 1){
        //do nothing
    }
    else{
        char temp;
        //for loop to print the array with indexes moved up (to the right) --> by 2
        for (int i=0; i < size; i++)
        {
            temp = myarray[size-shiftBy];
            myarray[size-shiftBy] = myarray[i];
            myarray[i] = temp;
        }

    }
}
void shiftLeft (char myarray[], int size, int shiftBy)
{
    if(shiftBy > size){
        shiftBy = shiftBy - size;
    }

    if(size == 1){
        //do nothing
    }
    else{
        char temp;
        //for loop to print the array with indexes moved up (to the left) <-- by 2
        for (int i=0; i < size-shiftBy; i++)
        {//EXAMPLE shift by 3  for a c-string of 5
            temp = myarray[i];//temp = myarray[0]
            myarray[i] = myarray[i + shiftBy];//myarray[0] == myarray[2]
            myarray[i + shiftBy] = temp;//myarray[2] = temp(value previously at index i)
        }

    }
}
void Display_menaya (int size,char *strnew) {
  int i=0;
  char *Ptr2Num[12] = {0};
    Ptr2Num[0] +=0xA29;//LCDM11_A1
    Ptr2Num[1] +=0xA2A;//LCDM10_A1
    Ptr2Num[2] +=0xA25;//LCDM6_A2
    Ptr2Num[3] +=0xA26;//LCDM7_A2
    Ptr2Num[4] +=0xA23;//LCDM4_A3
    Ptr2Num[5] +=0xA24;//LCDM5_A3
    Ptr2Num[6] +=0xA32;//LCDM19_A4
    Ptr2Num[7] +=0xA33;//LCDM20_A4
    Ptr2Num[8] +=0xA2E;//LCDM15_A5
    Ptr2Num[9] +=0xA2F;//LCDM16_A5
    Ptr2Num[10] +=0xA27;//LCDM8_A6
    Ptr2Num[11] +=0xA28;//LCDM9_A6
    for(int i=0;i<size;i++){
      if(strnew[i]=='0'){continue;}
    *Ptr2Num[i*2]=alphabetBig[(strnew[i]-65)][0];  
    *Ptr2Num[(i*2)+1]=alphabetBig[(strnew[i]-65)][1];
}
wait(1000);
 LCD_All_Off();
}


void clacVin(int vref,int vrefmin,int n){
  LCD_All_Off();
  long long Vin=0;
  Vin=(n*(vref-vrefmin))*0.244140625+vrefmin*1000;
  Display_Number(Vin);
}