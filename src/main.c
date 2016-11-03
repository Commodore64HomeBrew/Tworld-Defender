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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <mouse.h>
//#include "RsTypes.h"
#include <peekpoke.h>

#define bufSize 1000

#define PETSCII_STOP  0x03
//#define PETSCII_RUN   0x83

#define PETSCII_SPACE  0x20

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


#define DRIVER          "c64-1351.mou"


//typedef void (*TerminalFunc)( char pByte );
//static TerminalFunc pchar = 0;

static char heroChar = 170;
static char heroColour = 3;
static int heroPos;

static int screenH = 24;
static int screenW = 39;

static int screenPixH = 200;
static int screenPixW = 320;

static char sRunning = 1;

static char locBuf[bufSize][2] ;

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


static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};



//
// Functions

/*void pchar( unsigned char charin ){

  putchar(charin);

  return();

}*/

void moveShape(char type, char colour, int startPos)
{
  char n;
  int offset;

  for( n = 0; n < 4; n++ ){

    offset=startPos + shapes[type][n];

    POKE(0xd800 + offset , colour);
    locBuf[offset][0]=type;
    locBuf[offset][1]=n;
   }
}


void drawShape(char type, char colour, int startPos)
{
  char n;
  int offset;

  for( n = 0; n < 4; n++ ){

    offset=startPos + shapes[type][n];

    //POKE(0x0400 + offset , 224);
    POKE(0xd800 + offset , colour);
    locBuf[offset][0]=type;
    locBuf[offset][1]=n;
   }
}


/******************************************************************************/
/* screenRender                                                               */
/******************************************************************************/
void screenSetup()
{

      //draw shapes on screen. Each shape is separate function. There is a library of shapes and their occupation area. Each velocity vector is updated. If change is great enough, a new screen position is selected.
  int x,i;
  char s, c;

  bgcolor (0);
  bordercolor (0);

  clrscr ();

  for( i=0; i<1000; i++ )
  {
    POKE(0x0400 + i , 224);
    POKE(0xd800 + i , 0);
  }


  for( i=0; i<8; i++)
  {
    s = rand() % 6;
    c = rand() % 14;
    x = rand() % 1000;

    drawShape(s,c,x);
  }

  bgcolor (heroColour);

}



  



int main( void )
{

  struct mouse_info info;

  int i,s,c,x;
  int startPos,offset;

  char cursorx;
  char cursory;
  char pet;
  char selected;
  char curShape,curColour;

  memcpy ((void*) SPRITE0_DATA, MouseSprite0, sizeof (MouseSprite0));



  /* Load and install the mouse driver */
   //mouse_load_driver (&mouse_def_callbacks, DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;

    VIC.spr0_color = COLOR_WHITE;

  _randomize();

  screenSetup();


  cursorx=20;
  cursory=11;

  heroPos = (cursory)*40 + cursorx;
  POKE(0x0400 + heroPos , heroChar);
  
  mouse_show();
  mouse_move(cursorx , cursory);


  while( sRunning )
  {


    if( kbhit() )
      {
        //
        //uint8_t pet = cgetc();
        pet = cgetc();
        
        startPos = (cursory)*40 + cursorx;


        switch( pet )
        {
          //Move cursor down
          case PETSCII_DOWN:

            if(cursory < screenH){
              cursory++;
              POKE(0x0400 + heroPos , 224);
              heroPos = heroPos + 40;
              POKE(0x0400 + heroPos , heroChar);
            }
            break;

          //Move cursor up
          case PETSCII_UP:

            if(cursory > 0){
              cursory--;
              POKE(0x0400 + heroPos , 224);
              heroPos = heroPos - 40;
              POKE(0x0400 + heroPos , heroChar);
            }
            break;

          //Move cursor left
          case PETSCII_LEFT:

            if(cursorx > 0){
              cursorx--;
              POKE(0x0400 + heroPos , 224);
              heroPos = heroPos - 1;
              POKE(0x0400 + heroPos , heroChar);
            }
            break;

          //Move cursor right
          case PETSCII_RIGHT:

            if(cursorx < screenW){
              cursorx++;
              POKE(0x0400 + heroPos , 224);
              heroPos = heroPos + 1;
              POKE(0x0400 + heroPos , heroChar);
            }
            break;

          //Select object
          case PETSCII_F1:

            if(PEEK(0xd800 + heroPos) > 0 && selected == 0)
            {
              selected = 1;
              bgcolor (2);

              offset = shapes[  (locBuf[heroPos][0])  ][  (locBuf[heroPos][1])    ] - shapes[  (locBuf[heroPos][0])  ][  0  ];

              curShape = locBuf[heroPos][0];
              curColour = PEEK(0xd800 + heroPos);

            }
            else
            {
              selected = 0;
              bgcolor (heroColour);
            }

            break;



        }

        if(selected == 1)
        {


          drawShape(curShape, 0, startPos-offset);

          drawShape(curShape, curColour, heroPos-offset);

        }
        //mouse_move (cursorx , cursory);
      }







    // mouse_move (cursorx , cursory);

    /*for( i = 0; i < 4; i++ ){



    }*/
    //mouse_info (&info);

    //drawShape(0,1,info.pos.x*info.pos.y);



/*
  for( s=0; s<8; s++)
  {

    //for( c=1; c<15; c++)
    //{

      for( x=0; x<1000; x++)
      {
        //drawShape(s,0,x-1);
        drawShape(s,1,x);

        if(c>15){c=0;}
      }
   //}
  }

*/

  }




  return(0);
}
