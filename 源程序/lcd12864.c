#include<intrins.h>
#include"lcd12864.h"
#include"zt16x16.h"
#include"picture.h"


void delay(uint i)
{
	while(--i);
}

void read_busy()
{
	RS=0;
	RW=1;
	EN=1;
	while(LCD_Data&0x80);
	EN=0;
}
void write_com(uchar value)//写命令
{
	read_busy();
	RS=0;
	RW=0;
	EN=0;
	LCD_Data=value;
	EN=1;
	_nop_();
	_nop_();
	EN=0;
}
void write_dat(uchar value)//写数据
{
	read_busy();
	RS=1;
	RW=0;
	EN=0;
	LCD_Data=value;
	EN=1;
	_nop_();
	_nop_();
	EN=0;
}
void Set_page(uchar page)//设置页
{
	page=0xb8|page;
	write_com(page);
}
void Set_line(uchar line)//设置行
{
	line=0xc0|line;
	write_com(line);
}
void Set_column(uchar column)//设置列
{
	column=column&0x3f;
	column=0x40|column;
	write_com(column);
}
void Set_onoff(uchar onoff)//显示开关
{
	onoff=0x3e|onoff;
	write_com(onoff);
}
void SelectScreen(uchar screen)//选左右屏
{
	switch(screen)
	{
		case 0:CS1=1;CS2=1;break;
		case 1:CS1=1;CS2=0;break;
		case 2:CS1=0;CS2=1;break;
		default:break;
	}
}
void ClearScreen(uchar screen)
{
	uchar i,j;
	SelectScreen(screen);
	for(i=0;i<8;i++)
	{
		Set_page(i);
		Set_column(0);
		for(j=0;j<64;j++)
		{
			write_dat(0x00);
		}
	}
}
void initlcd()
{
	RST=0;
	RST=1;
	delay(1);
	read_busy();
	SelectScreen(0);
	Set_onoff(0);
	SelectScreen(0);
	Set_onoff(1);
	SelectScreen(0);
	ClearScreen(0);
	Set_line(0);
}


uint Asc2Unic(uchar *p)
{
	uint hh,hl;
	uint t;
	uint i;
	hh = p[0];
	hl = p[1];
	t = (hh<<8) | hl;
	for(i=0;i<sizeof(ASCtoUNIC);i++)
	{
		if(ASCtoUNIC[i]==t) return i;
	}
	return 0;
}
void DisplayHanziChar(uchar s,uchar page,uchar column,uchar *p)
{
	//uchar s,uchar page,uchar column,uchar *p
	uint dex;
	uchar i;

	dex = Asc2Unic(p);
	dex = dex * 32;

	SelectScreen(s);
	Set_page(page);//两页完成16个点，上半页
	Set_column(column);//选择列
	for(i=0;i<16;i++)
	{
		write_dat(st16[dex+i]);
	}
	Set_page(page+1);//下半页
	Set_column(column);
	for(i=0;i<16;i++)
	{
		write_dat(st16[dex+i+16]);
	}
}

void DisplayNumberChar(uchar s,uchar page,uchar column,uchar numdex)
{
	//uchar s,uchar page,uchar column,uchar *p
	uchar i;
	SelectScreen(s);
	Set_page(page);//两页完成16个点，上半页
	Set_column(column);//选择列
	for(i=0;i<8;i++)
	{
		write_dat(systemnum[numdex*16+i]);
	}
	Set_page(page+1);//下半页
	Set_column(column);
	for(i=0;i<8;i++)
	{
		write_dat(systemnum[numdex*16+8+i]);
	}
}

void DisplayNumberStr(uchar s,uchar page,uchar column,uint num,uchar n)
{
	uchar gw,sw,bw,qw;
	uint temp;
	temp = num%10000;
	qw = temp/1000;
	temp = num%1000;
	bw = temp/100;
	temp = num%100;
	sw = temp/10;
	gw = temp%10;
	switch(n)
	{
	case 1:
		DisplayNumberChar(s,page,(column+3)*8,gw);
		break;
	case 2:
		DisplayNumberChar(s,page,(column+3)*8,gw);
		DisplayNumberChar(s,page,(column+2)*8,sw);
		break;
	case 3:
		DisplayNumberChar(s,page,(column+3)*8,gw);
		DisplayNumberChar(s,page,(column+2)*8,sw);
		DisplayNumberChar(s,page,(column+1)*8,bw);
		break;
	case 4:
		DisplayNumberChar(s,page,(column+3)*8,gw);
		DisplayNumberChar(s,page,(column+2)*8,sw);
		DisplayNumberChar(s,page,(column+1)*8,bw);
		DisplayNumberChar(s,page,column,qw);
		break;
	default:
		break;
	}
}

void DisplayPicture(sroadblockmid midblock,tsplane plane,uchar* pbu,uchar* pbd,uchar dex)
{
	uchar i,j;

	#define planeXStart 48
	#define planeXStop 64
	uchar planeY,planeCheckY,planeData;

	uchar roadblockBesizeData;

	uchar roadblockMidderData;
	uchar midpageStart,midpageStop;

	planeCheckY = plane.height / 8;
	planeY = plane.height % 8;

	midpageStart = midblock.y/8;
	midpageStop = (midblock.y + midblock.h)/8;

	for(j=0;j<8;j++)
	{
		SelectScreen(1);
		Set_page(j);
		Set_column(32);
		write_dat(0xff);
		for(i=33;i<64;i++)
		{
			if((i>=planeXStart)&&(i<planeXStop)&&(j>=planeCheckY)&&(j<=(planeCheckY+1)))
			{	
				if(j==planeCheckY)
				{
					planeData = (Picture_Plane[plane.state*16+i-48]&planeLow[8-planeY])<<planeY;
				}
				else
				{
					planeData = (Picture_Plane[plane.state*16+i-48]&planeHight[planeY])>>(8-planeY);	
				}
			}
			else
			{
				planeData = 0x00;
			}

			if(j<4)
			{
				roadblockBesizeData = Picture_RoadblockUp[pbu[(i+dex-32)/4]*4+j];

			}
			else
			{
				roadblockBesizeData = Picture_RoadblockDown[pbd[(i+dex-32)/4]*4+j-4];
			}

			if((i>=midblock.x)&&(i<(midblock.x+midblock.w))&&(j>=midpageStart)&&(j<=midpageStop))
			{	
				if(j==midpageStart)
				{
					roadblockMidderData = 0xff<<(midblock.y%8);
				}
				else if((j>midpageStart)&&(j<midpageStop))
				{
					roadblockMidderData = 0xff;	
				}
				else
				{
					roadblockMidderData = 0xff>>(8-((midblock.y+midblock.h)%8));
				}
			}
			else
			{
				roadblockMidderData = 0x00;
			}

			write_dat(planeData | roadblockBesizeData | roadblockMidderData);
		}

		SelectScreen(2);
		Set_page(j);
		Set_column(0);
		for(i=64;i<128;i++)
		{
			if(j<4)
			{
				roadblockBesizeData = Picture_RoadblockUp[pbu[(i-64+dex)/4+8]*4+j];
			}
			else
			{
				roadblockBesizeData = Picture_RoadblockDown[pbd[(i-64+dex)/4+8]*4+j-4];
			}

			if((i>=midblock.x)&&(i<(midblock.x+midblock.w))&&(j>=midpageStart)&&(j<=midpageStop))
			{	
				if(j==midpageStart)
				{
					roadblockMidderData = 0xff<<(midblock.y%8);
				}
				else if((j>midpageStart)&&(j<midpageStop))
				{
					roadblockMidderData = 0xff;	
				}
				else
				{
					roadblockMidderData = 0xff>>(8-((midblock.y+midblock.h)%8));
				}
			}
			else
			{
				roadblockMidderData = 0x00;
			}

			write_dat(roadblockBesizeData | roadblockMidderData);
		}
	}
}