#pragma once

#include "device.h"

#define COG_TIMER_OFF -1
#define COG_TIMER_ON 0
#define COG_TIMER_END 1

typedef signed int timer_status;

struct EventRegisterData
{
	double EventData[6];
};

struct Timer
{
	UINT32 Ticks;
	UINT32 Limit;
	BOOL Active;
};

union cogptr
{
	int i;
	double d;
	cogptr(int);
	cogptr(double);
};

typedef void(*CogFireCallback)(EventRegisterData*);

class PineDevice;
class CogBytecode;

class PineCog
{
public:	
	PineCog(PineDevice*, UINT32, CogBytecode*);
	~PineCog();

	void RegisterFireEventCallback(CogFireCallback cb);
	PINERESULT Iterate();
	double out();
	PINERESULT setreg(UINT32, double);
	PINERESULT getreg(UINT32, double*);

private:
	int _readpos;
	BOOL read_int(INT32&);
	BOOL read_byte(byte&);
	BOOL read_double(double&);
	template <typename T>
	T read(T&);
	
	EventRegisterData* evdata();
	void fire();
	CogFireCallback _cbFire;
	
	UINT32 _period;
	double _output;

	CogBytecode* _code;
	PineDevice* _device;

	double* _registers; // REGISTER_COUNT	
	Timer** _timers;
	void tset(UINT32 i, UINT32 v);
	void tstart(UINT32 i);
	void tstop(UINT32 i);
	timer_status tstatus(UINT32 i);
	void treset(UINT32 i);
	UINT32 tgett(UINT32 i);
	UINT32 tgetl(UINT32 i);
	void dotimers();

	INT32 r0, r1;
	UINT32 u0, u1;
	double va, vb, vc, rmin, rmax, x;
	BOOL ranged;	
	byte ci;
};