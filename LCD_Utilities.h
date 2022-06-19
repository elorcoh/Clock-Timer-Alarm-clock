#include "msp430.h"
#include <string.h>
#ifndef INC
#define INC
#define HRT_ON  (LCDM3 |= BIT2);
#define HRT_OFF (LCDM3 &= ~BIT2);
#define SIGN_LESS_ON (LCDM11 |= NEG);
#define SIGN_LESS_OFF (LCDM11 &= ~NEG);
#define SIGN_ERR_ON (LCDM8 = ERR);
#define A1_0 LCDM10= 0xFC
#define A1_1 LCDM10= 0x60
#define A1_2 LCDM10= 0xDB
#define A1_3 LCDM10= 0xF3
#define A1_4 LCDM10= 0x67
#define A1_5 LCDM10= 0xB7
#define A1_6 LCDM10= 0xBF
#define A1_7 LCDM10= 0xE4
#define A1_8 LCDM10= 0xFF
#define A1_9 LCDM10= 0xF7 
#define A2_0 LCDM6= 0xFC
#define A2_1 LCDM6= 0x60
#define A2_2 LCDM6= 0xDB
#define A2_3 LCDM6= 0xF3
#define A2_4 LCDM6= 0x67
#define A2_5 LCDM6= 0xB7
#define A2_6 LCDM6= 0xBF
#define A2_7 LCDM6= 0xE4
#define A2_8 LCDM6= 0xFF
#define A2_9 LCDM6= 0xF7 

#define A3_0 LCDM4= 0xFC
#define A3_1 LCDM4= 0x60
#define A3_2 LCDM4= 0xDB
#define A3_3 LCDM4= 0xF3
#define A3_4 LCDM4= 0x67
#define A3_5 LCDM4= 0xB7
#define A3_6 LCDM4= 0xBF
#define A3_7 LCDM4= 0xE4
#define A3_8 LCDM4= 0xFF
#define A3_9 LCDM4= 0xF7 

#define A4_0 LCDM19= 0xFC
#define A4_1 LCDM19= 0x60
#define A4_2 LCDM19= 0xDB
#define A4_3 LCDM19= 0xF3
#define A4_4 LCDM19= 0x67
#define A4_5 LCDM19= 0xB7
#define A4_6 LCDM19= 0xBF
#define A4_7 LCDM19= 0xE4
#define A4_8 LCDM19= 0xFF
#define A4_9 LCDM19= 0xF7 

#define A5_0 LCDM15= 0xFC
#define A5_1 LCDM15= 0x60
#define A5_2 LCDM15= 0xDB
#define A5_3 LCDM15= 0xF3
#define A5_4 LCDM15= 0x67
#define A5_5 LCDM15= 0xB7
#define A5_6 LCDM15= 0xBF
#define A5_7 LCDM15= 0xE4
#define A5_8 LCDM15= 0xFF
#define A5_9 LCDM15= 0xF7 

#define A6_0 LCDM8= 0xFC
#define A6_1 LCDM8= 0x60
#define A6_2 LCDM8= 0xDB
#define A6_3 LCDM8= 0xF3
#define A6_4 LCDM8= 0x67
#define A6_5 LCDM8= 0xB7
#define A6_6 LCDM8= 0xBF
#define A6_7 LCDM8= 0xE4
#define A6_8 LCDM8= 0xFF
#define A6_9 LCDM8= 0xF7
void LCDInit ();
void LCD_All_On();
void LCD_All_Off();
void Display_Number (long long n);
void Display_digit(int p,int q);
void Display_letter (int size,char *letter);
void Display_menaya (int size,char *letter);
void wait(int ms);
void shiftright (char *myarray, int size, int shiftBy);
void shiftLeft (char myarray[], int size, int shiftBy);
#endif