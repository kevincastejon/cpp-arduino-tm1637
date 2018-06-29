/*
 * TM1637.cpp
 * A library for the 4 digit display
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
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
 */

#include "TM1637.h"
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f, //0-9
                           0x77,0x5f, //A-a
                           0x7f,0x7c, //B-b
                           0x39,0x58, //C-c
                           0x3f,0x5e, //D-d
                           0x79,0x7b, //E-e
                           0x71,0x71, //F-f
                           0x3d,0x6f, //G-g
                           0x76,0x74, //H-h
                           0x06,0x04, //I-i
                           0x1e,0x0e, //J-j
                           0x76,0x74, //K-k
                           0x38,0x06, //L-l
                           0x37,0x54, //M-m
                           0x76,0x54, //N-n
                           0x3f,0x5c, //O-o
                           0x73,0x73, //P-p
                           0x67,0x67, //Q-q
                           0x77,0x50, //R-r
                           0x6d,0x6d, //S-s
                           0x78,0x78, //T-t
                           0x3e,0x1c, //U-u
                           0x3e,0x1c, //V-v
                           0x3e,0x1c, //W-w
                           0x76,0x76, //X-x
                           0x66,0x66, //Y-y
                           0x5b,0x5b, //Z-z
                           0x00,      //" "
                           0x40,0x48, //-,=
                           0x39,0x0f, //[,]
                           0x39,0x0f //(,)
                         };
static char TubeTabChars[] = {
'0',
'1',
'2',
'3',
'4',
'5',
'6',
'7',
'8',
'9',
'A',
'a',
'B',
'b',
'C',
'c',
'D',
'd',
'E',
'e',
'F',
'f',
'G',
'g',
'H',
'h',
'I',
'i',
'J',
'j',
'K',
'k',
'L',
'l',
'M',
'm',
'N',
'n',
'O',
'o',
'P',
'p',
'Q',
'q',
'R',
'r',
'S',
's',
'T',
't',
'U',
'u',
'V',
'v',
'W',
'w',
'X',
'x',
'Y',
'y',
'Z',
'z',
' ',
'-',
'=',
'[',
']',
'(',
')'
};
TM1637::TM1637(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1637::init(void)
{
  clearDisplay();
}

int TM1637::writeByte(int8_t wr_data)
{
  uint8_t i,count1;
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;
    digitalWrite(Clkpin,HIGH);

  }
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);
  pinMode(Datapin,INPUT);

  bitDelay();
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0)
  {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
  }
  bitDelay();
  pinMode(Datapin,OUTPUT);
  bitDelay();

  return ack;
}
//send start signal to TM1637
void TM1637::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,LOW);
}
//End of transmission
void TM1637::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH);
}
void TM1637::show(uint8_t BitAddr,char cha)
{
  int ind=-1;
  for (size_t i = 0; i < sizeof(TubeTabChars); i++) {
    if(TubeTabChars[i]==cha){
    ind=i;break;}
  }
display(BitAddr,ind);
}
void TM1637::show(char* cha)
{
  int max=strlen(cha);
  int dif=4-max;
  char ret[4]="    ";
  for (size_t j = 0; j < max; j++) {
    ret[j+dif]=cha[j];
  }
  for (size_t i = 0; i < 4; i++) {
    show(i,ret[i]);
  }
}
//display function.Write to full-screen
void TM1637::display(int8_t DispData[])
{
  int8_t SegData[4];
  uint8_t i;
  for(i = 0;i < 4;i ++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(Cmd_SetAddr);//
  for(i=0;i < 4;i ++)
  {
    writeByte(SegData[i]);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}
//******************************************
void TM1637::display(uint8_t BitAddr,int8_t DispData)
{
  int8_t SegData;
  SegData = coding(DispData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(BitAddr|0xc0);//
  writeByte(SegData);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

void TM1637::clearDisplay(void)
{
  display(0x00,0x7f);
  display(0x01,0x7f);
  display(0x02,0x7f);
  display(0x03,0x7f);
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(boolean PointFlag)
{
  _PointFlag = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0;
  for(uint8_t i = 0;i < 4;i ++)
  {
    if(DispData[i] == 0x7f)DispData[i] = 0x00;
    else DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t TM1637::coding(int8_t DispData)
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0;
  if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + PointData;
  return DispData;
}
void TM1637::bitDelay(void)
{
	delayMicroseconds(50);
}
