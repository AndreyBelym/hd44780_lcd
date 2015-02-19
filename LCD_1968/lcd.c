// LCD.c
// Runs on LM3S1968
// Simple device driver for the LCD
// Valvano
// August 2, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

   Section 4.7.1, Program 4.2 and Program 4.3

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

/*
  size is 1*8
  if do not need to read busy, then you can tie R/W=ground
  ground = pin 1    Vss
  power  = pin 2    Vdd   +3.3V or +5V depending on the device
  ground = pin 3    Vlc   grounded for highest contrast
  PG0    = pin 4    RS    (1 for data, 0 for control/status)
  PG1    = pin 5    R/W   (1 for read, 0 for write)
  PG2    = pin 6    E     (enable)
  PF0    = pin 7    DB0   (8-bit data)
  PF1    = pin 8    DB1
  PF2    = pin 9    DB2
  PF3    = pin 10   DB3
  PF4    = pin 11   DB4
  PF5    = pin 12   DB5
  PF6    = pin 13   DB6
  PF7    = pin 14   DB7
8 characters are configured as 1 rows of 8
addr  00 01 02 03 04 05 06 07
*/

#include "LCD.h"
#include "lm3S1968.h"
#include "systick.h"

#define E  4 // on PG2
#define RW 2 // on PG1
#define RS 1 // on PG0
#define LCDDATA (*((volatile unsigned long *)0x400253FC))   // PORTF
#define LCDCMD (*((volatile unsigned long *)0x4002601C))    // PG2-PG0
#define BusFreq 50            // assuming a 50 MHz bus clock
#define T6us 6*BusFreq        // 6us
#define T40us 40*BusFreq      // 40us
#define T160us 160*BusFreq    // 160us
#define T1600us 1600*BusFreq  // 1.60ms
#define T5ms 5000*BusFreq     // 5ms
#define T15ms 15000*BusFreq   // 15ms
void OutCmd(unsigned char command){
  LCDDATA = command;
  LCDCMD = 0;           // E=0, R/W=0, RS=0
  SysTick_Wait(T6us);   // wait 6us
  LCDCMD = E;           // E=1, R/W=0, RS=0
  SysTick_Wait(T6us);   // wait 6us
  LCDCMD = 0;           // E=0, R/W=0, RS=0
  SysTick_Wait(T40us);  // wait 40us
}

// Initialize LCD
// Inputs: none
// Outputs: none
void LCD_Init(void){ volatile long delay;
  SYSCTL_RCGC2_R |= 0x00000060;  // 1) activate clock for Ports F and G
  delay = SYSCTL_RCGC2_R;        // allow time for clock to start
  GPIO_PORTF_DIR_R = 0xFF;       // 2) set direction register
  GPIO_PORTG_DIR_R |= 0x07;
  GPIO_PORTF_AFSEL_R = 0x00;     // 3) regular port function
  GPIO_PORTG_AFSEL_R &= ~0x07;
  GPIO_PORTF_DEN_R = 0xFF;       // 4) enable digital port
  GPIO_PORTG_DEN_R |= 0x07;
  GPIO_PORTF_DR8R_R = 0xFF;      // 5) enable 8 mA drive
  GPIO_PORTG_DR8R_R |= 0x07;
  SysTick_Init();       // Program 2.10
  LCDCMD = 0;           // E=0, R/W=0, RS=0
  SysTick_Wait(T15ms);  // Wait >15 ms after power is applied
  OutCmd(0x30);         // command 0x30 = Wake up
  SysTick_Wait(T5ms);   // must wait 5ms, busy flag not available
  OutCmd(0x30);         // command 0x30 = Wake up #2
  SysTick_Wait(T160us); // must wait 160us, busy flag not available
  OutCmd(0x30);         // command 0x30 = Wake up #3
  SysTick_Wait(T160us); // must wait 160us, busy flag not available
  OutCmd(0x38);         // Function set: 8-bit/2-line
  OutCmd(0x10);         // Set cursor
  OutCmd(0x0c);         // Display ON; Cursor ON
  OutCmd(0x06);         // Entry mode set
}
// Output a character to the LCD
// Inputs: letter is ASCII character, 0 to 0x7F
// Outputs: none
void LCD_OutChar(unsigned char letter){
  LCDDATA = letter;
  LCDCMD = RS;          // E=0, R/W=0, RS=1
  SysTick_Wait(T6us);   // wait 6us
  LCDCMD = E+RS;        // E=1, R/W=0, RS=1
  SysTick_Wait(T6us);   // wait 6us
  LCDCMD = RS;          // E=0, R/W=0, RS=1
  SysTick_Wait(T40us);  // wait 40us
}

// Clear the LCD
// Inputs: none
// Outputs: none
void LCD_Clear(void){
  OutCmd(0x01);          // Clear Display
  SysTick_Wait(T1600us); // wait 1.6ms
  OutCmd(0x02);          // Cursor to home
  SysTick_Wait(T1600us); // wait 1.6ms
}

//------------LCD_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void LCD_OutString(char *pt){
  while(*pt){
    LCD_OutChar(*pt);
    pt++;
  }
}

//-----------------------LCD_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void LCD_OutUDec(unsigned long n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    LCD_OutUDec(n/10);
    n = n%10;
  }
  LCD_OutChar(n+'0'); /* n is between 0 and 9 */
}

//--------------------------LCD_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void LCD_OutUHex(unsigned long number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    LCD_OutUHex(number/0x10);
    LCD_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      LCD_OutChar(number+'0');
     }
    else{
      LCD_OutChar((number-0x0A)+'A');
    }
  }
}
