#ifndef __HARDCONFIG_H__
#define __HARDCONFIG_H__

#include<reg52.h>
#include"mydefine.h"


sbit RS=P1^0;  
sbit RW=P1^1;   
sbit EN=P1^2; 
sbit CS1=P2^6;   
sbit CS2=P2^5;   
sbit RST=P2^4;

sbit KEY=P2^0;
sbit pwm=P2^1;
#define LCD_Data P0


#endif