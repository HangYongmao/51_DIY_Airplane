#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__

struct TIMESTRUCT{
	uchar time_planestate;
	uchar time_display;
	uchar time_keycheck;
}stime;

struct FLAGSTRUCT{
	uchar flag_planestate;
	uchar flag_display;
	uchar flag_keycheck;
}sflag;

struct PLANE{
	uchar height;
	uchar state;
	uchar lives;
	uint  distance;
}splane;

#endif