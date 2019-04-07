#include"lcd12864.h"

code uchar str_kaijidonghua[]="开机动画";
code uchar str_youximingchen[]="游戏名称";
code uchar str_dixinmaoxianji[]="地心冒险记";
code uchar str_niguale[]="你挂了";
code uchar anjianchonglai[]="按键重来";

tstime stime;
tsflag sflag;
tsplane splane;
sroadblockmid roadblockmid;
uchar roadblockup[25];
uchar roadblockdown[25];
uchar roadblockmove1;

uint topScore=0;

uchar Max(uchar x1,uchar x2)
{
	if(x1>x2)
	{
		return x1;
	}
	else
	{
		return x2;
	}
}

uchar Min(uchar x1,uchar x2)
{
	if(x1<x2)
	{
		return x1;
	}
	else
	{
		return x2;
	}
}

uchar CrashCheckMidder()
{
	uchar minx,miny,maxx,maxy;
	uchar minx1,miny1,maxx1,maxy1;
	uchar minx2,miny2,maxx2,maxy2;

	minx1 = 48;
	miny1 = splane.height;
	maxx1 = 63;
	maxy1 = splane.height + 8;

	minx2 = roadblockmid.x;
	miny2 = roadblockmid.y;
	maxx2 = roadblockmid.x + roadblockmid.w - 1;
	maxy2 = roadblockmid.y + roadblockmid.h ;

	minx = Max(minx1,minx2);
	miny = Max(miny1,miny2);
	maxx = Min(maxx1,maxx2);
	maxy = Min(maxy1,maxy2);

	if((minx<maxx)&&(miny<maxy))
		return 1;
	return 0;
}

uchar CrashCheck()
{
	uchar i;
	uchar temp;

	if(roadblockmove1>0)
	{
		temp = 8;
	}
	else
	{
		temp = 7;
	}
	for(i=4;i<temp;i++)
	{
		if(roadblockup[i]>splane.height)
		{
			return 1;
		}
		if((64-roadblockdown[i])<(splane.height)+8)
		{
			return 1;
		}
	}

	if(CrashCheckMidder()>0)
		return 1;

	return 0;
}

void Roadblockmove()
{
	uchar i;
//	uchar tmax,tt;
	uchar th,tl;
	uchar tpass;

	tl = TL0;
	th = TH0;

	splane.distance++;
	if(splane.distance>9999)
	{
		splane.distance = 0;
	}

	if(roadblockmid.x<(33-roadblockmid.w))
	{
		roadblockmid.x = 128;
		roadblockmid.y = (tl%32)+8;
		roadblockmid.w = (th>>1)%32+1;
		roadblockmid.h = th%16+1;
	}
	roadblockmid.x--;

	roadblockmove1++;
	if(roadblockmove1>3)
	{
		roadblockmove1 = 0;

		tl = tl%8;
		th = th%8;

		tpass = splane.distance/1250;

		for(i=0;i<24;i++)
		{
			roadblockup[i] = roadblockup[i+1];
		}
		if(roadblockup[23]<8)
		{
			roadblockup[24]	 = 	roadblockup[23] + tl;
		}
		else if(roadblockup[23]>(tpass*2+8))
		{
			roadblockup[24]	 = 	roadblockup[23] - tl;
		}
		else
		{
			roadblockup[24]	 = 	roadblockup[23] + th -tl;
		}
		if(roadblockup[23]+roadblockdown[23]>=53)
		{
			roadblockup[24] -= 8;
		}

		for(i=0;i<24;i++)
		{
			roadblockdown[i] = roadblockdown[i+1];
		}
		if(roadblockdown[23]<8)
		{
			roadblockdown[24]	 = 	roadblockdown[23] + th;
		}
		else if(roadblockdown[23]>(tpass*2+8))
		{
			roadblockdown[24]	 = 	roadblockdown[23] - th;
		}
		else
		{
			roadblockdown[24]	 = 	roadblockdown[23] + tl -th;
		}

		if(roadblockdown[23]+roadblockdown[23]>=53)
		{
			roadblockdown[24] -= 8;
		}
	}
}

void Display()
{
	DisplayPicture(roadblockmid,splane,roadblockup,roadblockdown,roadblockmove1);

	DisplayHanziChar(1,0,0*16,"航");
	DisplayHanziChar(1,0,1*16,"程");
	DisplayNumberStr(1,2,0,splane.distance,4);
	DisplayHanziChar(1,4,0*16,"最");
	DisplayHanziChar(1,4,1*16,"高");
	DisplayNumberStr(1,6,0,topScore,4);
}

void time_init()
{
	//方式0，1mS定时，开中断
	TMOD = 0x00;
	TH0 = 0x7d;
	TL0 = 0x19;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void IOinit()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
}

void SystermInit()
{
	uchar i;

	ClearScreen(0);
	DisplayHanziChar(1,0,1*16,str_youximingchen);
	DisplayHanziChar(1,0,2*16,str_youximingchen+2);
	DisplayHanziChar(1,0,3*16,str_youximingchen+4);
	DisplayHanziChar(2,0,0*16,str_youximingchen+6);
	DisplayHanziChar(2,0,1*16,"：");

	DisplayHanziChar(1,3,2*16,str_dixinmaoxianji);
	DisplayHanziChar(1,3,3*16,str_dixinmaoxianji+2);
	DisplayHanziChar(2,3,0*16,str_dixinmaoxianji+4);
	DisplayHanziChar(2,3,1*16,str_dixinmaoxianji+6);
	DisplayHanziChar(2,3,2*16,str_dixinmaoxianji+8);

	for(i=0;i<20;i++)
	delay(65535);

	splane.height = 0;
	splane.state = 0;
	splane.distance = 0;
	roadblockmove1 = 0;
//	topScore = 0;
	for(i=0;i<24;i++)
	{
		roadblockup[i] = 0;
		roadblockdown[i] = 0;
	}
	roadblockmid.w = 8;
	roadblockmid.h = 8;
	roadblockmid.y = 32;
	roadblockmid.x = 128;
}

void main()
{
	uint i;
	uchar l=0;

	char h=0;

	IOinit();
	initlcd();
	ClearScreen(0);

	DisplayHanziChar(1,3,2*16,str_kaijidonghua);
	DisplayHanziChar(1,3,3*16,str_kaijidonghua+2);
	DisplayHanziChar(2,3,0*16,str_kaijidonghua+4);
	DisplayHanziChar(2,3,1*16,str_kaijidonghua+6);
	for(i=0;i<5;i++)
	delay(65535);

	SystermInit();
	time_init();

	while(1)
	{
		if(sflag.flag_roadblockmove==1)
		{
			sflag.flag_roadblockmove = 0;

			if(CrashCheck()>0)
			{
				DisplayHanziChar(2,3,1*16,str_niguale);
				DisplayHanziChar(2,3,2*16,str_niguale+2);
				DisplayHanziChar(2,3,3*16,str_niguale+4);

				if(topScore<splane.distance)
				{
					topScore = splane.distance;
				}

				for(i=0;i<10;i++)
					delay(65535);

				DisplayHanziChar(2,6,0*16,anjianchonglai);
				DisplayHanziChar(2,6,1*16,anjianchonglai+2);
				DisplayHanziChar(2,6,2*16,anjianchonglai+4);
				DisplayHanziChar(2,6,3*16,anjianchonglai+6);

				while(KEY);

				SystermInit();
				continue;
			}
			Roadblockmove();
		}

		if(sflag.flag_planestate==1)
		{
			sflag.flag_planestate = 0;
			splane.state++;
			if(splane.state>3)
			{
				splane.state = 0;
			}
		}

		if(sflag.flag_display==1)
		{
			sflag.flag_display = 0;
			Display();
		}

		if(sflag.flag_keycheck==1)
		{
			sflag.flag_keycheck = 0;

			if(KEY==0)
			{
				h++;
				if(h>2)
				{
					h = 0;
					if(splane.height!=0)
					{
						splane.height--;
					}
				}
			}
			else
			{
				h--;
				if(h<-2)
				{
					h = 0;
					splane.height++;
					if(splane.height>55)
					{
					splane.height = 55;
					}
				}					
			}
		}
	}
}

//================interupt=====================================

void time0_interrupt()  interrupt 1 using 2
{
	TH0 = 0x7d;
	TL0 = 0x19;

	pwm = ~pwm;

	stime.time_roadblockmove++;
	if(stime.time_roadblockmove>50)
	{
		stime.time_roadblockmove = 0;
		sflag.flag_roadblockmove = 1;
	}

	stime.time_planestate++;
	if(stime.time_planestate>100)
	{
		stime.time_planestate = 0;
		sflag.flag_planestate = 1;
	}

	stime.time_display++;
	if(stime.time_display>0)
	{
		stime.time_display = 0;
		sflag.flag_display = 1;
	}

	stime.time_keycheck++;
	if(stime.time_keycheck>0)
	{
		stime.time_keycheck = 0;
		sflag.flag_keycheck = 1;
	}
}