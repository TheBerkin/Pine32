#pragma once

#include "device.h"

struct EventRegisterData
{
	double EventData[6];
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
	BOOL eos();
	int _readpos;
	BOOL read_int(INT32&);
	BOOL read_byte(byte&);
	BOOL read_double(double&);
	
	EventRegisterData* evdata();
	void fire();
	CogFireCallback _cbFire;
	
	UINT32 _period;
	double _output;

	CogBytecode* _code;
	PineDevice* _device;

	double* _registers; // REGISTER_COUNT	
	INT32 r0, r1;
	double va, vb, vc, rmin, rmax, x;
	BOOL ranged;	
	byte ci;
};