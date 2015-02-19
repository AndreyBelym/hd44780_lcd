// LCDTestMain.c
// Runs on LM3S1968
// Used to test the LCD.c driver
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

#include "hw_types.h"
#include "sysctl.h"
#include "LCD.h"
#include "Systick.h"

//debug code
int main(void){
  unsigned long n;
  //
  // Set the clocking to run at 50MHz from the PLL.
  //
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  LCD_Init();              // initialize LCD
  n = 0;
  while(1){
    LCD_Clear();
    LCD_OutString("Test LCD");
    SysTick_Wait10ms(100);
    LCD_Clear();
    LCD_OutUDec(n);
    SysTick_Wait10ms(100);
    LCD_OutChar(32);
    SysTick_Wait10ms(100);
    LCD_OutUHex(n);
    SysTick_Wait10ms(100);
    n++;
  }
}
