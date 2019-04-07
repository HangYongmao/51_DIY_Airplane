#ifndef __LCD12864_H__
#define __LCD12864_H__

#include"hardcofig.h"

extern void delay(uint i);
void read_busy();
void write_com(uchar value);//写命令
void write_dat(uchar value);//写数据
void Set_page(uchar page);//设置页
void Set_line(uchar line);//设置行
void Set_column(uchar column);//设置列
void Set_onoff(uchar onoff);//显示开关
void SelectScreen(uchar screen);//选左右屏
void ClearScreen(uchar screen);//清屏
extern void initlcd();
uint Asc2Unic(uchar *p);
extern  void DisplayHanziChar(uchar s,uchar page,uchar column,uchar *p);
extern void DisplayNumberChar(uchar s,uchar page,uchar column,uchar numdex);
extern void DisplayNumberStr(uchar s,uchar page,uchar column,uint num,uchar n);
extern void DisplayPicture(sroadblockmid midblock,tsplane plane,uchar* pbu,uchar* pbd,uchar dex);
#endif