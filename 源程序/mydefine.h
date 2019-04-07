#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

#define uint unsigned int
#define uchar unsigned char

typedef struct TIMESTRUCT{
	uchar time_planestate;
	uchar time_display;
	uchar time_keycheck;
	uchar time_roadblockmove;
}tstime;

typedef struct FLAGSTRUCT{
	uchar flag_planestate;
	uchar flag_display;
	uchar flag_keycheck;
	uchar flag_roadblockmove;
}tsflag;

typedef struct PLANE{
	uchar height;
	uchar state;
	uint  distance;
}tsplane;

typedef struct ROADBLOCKMIDDERSTRUCT{
	uchar x;
	uchar y;
	uchar w;
	uchar h;
}sroadblockmid;

#endif