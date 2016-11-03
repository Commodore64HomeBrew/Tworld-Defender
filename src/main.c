/**
* The MIT License (MIT)
* 
* Copyright (c) 2015 HyperSequence
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

//
// Includes
#include <cbm.h>
//#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <mouse.h>
//#include "RsTypes.h"
//#include <peekpoke.h>

#define bufSize 5000

#define PETSCII_STOP  0x03
//#define PETSCII_RUN   0x83

#define PETSCII_F1  0x85
//#define PETSCII_F2  0x89
#define PETSCII_F3  0x86
//#define PETSCII_F4  0x8a
#define PETSCII_F5  0x87
//#define PETSCII_F6  0x8b
#define PETSCII_F7  0x88
//#define PETSCII_F8  0x8c

#define PETSCII_RVSON  0x12
#define PETSCII_RVSOFF 0x92

#define PETSCII_DOWN  0x11
#define PETSCII_UP    0x91
#define PETSCII_RIGHT 0x1D
#define PETSCII_LEFT  0x9D


#define SPRITE0_DATA    0x0340
#define SPRITE0_PTR     0x07F8


#define DRIVER          "c64-ptvjoy.joy"


//typedef void (*TerminalFunc)( char pByte );
//static TerminalFunc pchar = 0;


static char shapes={0,1,40,41};



static int screenH = 24;
static int screenW = 40;

static int screenPixH = 200;
static int screenPixW = 320;

static char url[64];
static char sRecvBuf[bufSize] ;

static int linkTable[40][4];

static char sRunning = 1;

/* The mouse sprite (an arrow) */
static const unsigned char MouseSprite0[64] = {
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x0F, 0xE0, 0x00,
    0x0F, 0xC0, 0x00,
    0x0F, 0x80, 0x00,
    0x0F, 0xC0, 0x00,
    0x0D, 0xE0, 0x00,
    0x08, 0xF0, 0x00,
    0x00, 0x78, 0x00,
    0x00, 0x3C, 0x00,
    0x00, 0x1E, 0x00,
    0x00, 0x0F, 0x00,
    0x00, 0x07, 0x80,
    0x00, 0x03, 0x80,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00
};



//
// Functions

void pchar( unsigned char charin ){

  putchar(charin);

}


/******************************************************************************/
/* screenRender                                                               */
/******************************************************************************/
unsigned char screenSetup( int indx )
{

      //draw shapes on screen. Each shape is separate function. There is a library of shapes and their occupation area. Each velocity vector is updated. If change is great enough, a new screen position is selected.

  bgcolor (0);
  bordercolor (0);

  clrscr ();
  
  return();

}


drawB1(char x, char y, char t)
{



void print(char type, char colour, int position)
{
  char n;

  for( n = 0; n < 4; n++ ){

    offset=position+shapes[type][n];

    POKE(0x0400+offset,224);
    POKE(0xd800+offset,colour);
   }
 


}

  



int main( void )
{

  int i = 0;
  int bufIndx;
  int bufPage;
  int charCnt;
  char cursorx;
  char cursory;


  memcpy ((void*) SPRITE0_DATA, MouseSprite0, sizeof (MouseSprite0));



  /* Load and install the mouse driver */
   mouse_load_driver (&mouse_def_callbacks, DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;

    VIC.spr0_color = COLOR_WHITE;



  cursorx=20;
  cursory=11;

  mouse_show();
  mouse_move(cursorx , cursory);


  screenSetup();

  while( sRunning )
  {


           // mouse_move (cursorx , cursory);


  }



  return(0);
}
