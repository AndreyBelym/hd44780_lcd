// LCD.h
// Runs on LM3S1968
// header file for the LCD
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

// Initialize LCD
// Inputs: none
// Outputs: none
void LCD_Init(void);

// Output a character to the LCD
// Inputs: letter is ASCII character, 0 to 0x7F
// Outputs: none
void LCD_OutChar(unsigned char letter);

// Clear the LCD
// Inputs: none
// Outputs: none
void LCD_Clear(void);

//------------LCD_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void LCD_OutString(char *pt);

//-----------------------LCD_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void LCD_OutUDec(unsigned long n);

//--------------------------LCD_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void LCD_OutUHex(unsigned long number);
