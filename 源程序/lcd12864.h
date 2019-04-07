#ifndef __LCD12864_H__
#define __LCD12864_H__

#include"hardcofig.h"

extern void delay(uint i);
void read_busy();
void write_com(uchar value);//д����
void write_dat(uchar value);//д����
void Set_page(uchar page);//����ҳ
void Set_line(uchar line);//������
void Set_column(uchar column);//������
void Set_onoff(uchar onoff);//��ʾ����
void SelectScreen(uchar screen);//ѡ������
void ClearScreen(uchar screen);//����
extern void initlcd();
uint Asc2Unic(uchar *p);
extern  void DisplayHanziChar(uchar s,uchar page,uchar column,uchar *p);
extern void DisplayNumberChar(uchar s,uchar page,uchar column,uchar numdex);
extern void DisplayNumberStr(uchar s,uchar page,uchar column,uint num,uchar n);
extern void DisplayPicture(sroadblockmid midblock,tsplane plane,uchar* pbu,uchar* pbd,uchar dex);
#endif