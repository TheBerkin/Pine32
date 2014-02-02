#include "pine32.h"

#include <iostream>
#include <fstream>
#include<Windows.h>

using namespace std;

PINEAPI(PineDevice*) pineCreateDevice()
{
	return new PineDevice();
}

PINEAPI(void) pineDestroyDevice(PineDevice *lpDevice)
{
	delete lpDevice;
}

PINEAPI(PINERESULT) pineStepDevice(PineDevice *lpDevice)
{
	return lpDevice->Step();
}

PINEAPI(BOOL) pineGetEnabled(PineDevice *lpDevice)
{
	return lpDevice->GetEnabled();
}

PINEAPI(void) pineDisable(PineDevice *lpDevice)
{
	lpDevice->Disable();
}

PINEAPI(void) pineEnable(PineDevice *lpDevice)
{
	lpDevice->Enable();
}

PINEAPI(PINERESULT) pineCreateCog(PineDevice* device, CogBytecode* bytecode, int period, PineCog* lpCogOut)
{
	PineCog* ohboy = new PineCog(device, period, bytecode);
	int p = 0;
	PINERESULT result = device->Allocate(ohboy, &p);
	if (result != PINE_OK)
	{
		lpCogOut = nullptr;
		delete ohboy;
		ohboy = nullptr;
	}
	else
	{
		lpCogOut = ohboy;
	}
	return result;
}

PINEAPI(PINERESULT) pineDestroyCog(PineDevice* device, PineCog* cog)
{
	return device->DeallocateObj(cog);
}

PINEAPI(PINERESULT) pineSetCogRegister(PineCog* cog, UINT32 reg, double value)
{
	return cog->setreg(reg, value);
}

PINEAPI(PINERESULT) pineGetCogRegister(PineCog* cog, UINT32 reg, double* valueOut)
{
	return cog->getreg(reg, valueOut);
}

PINEAPI(double) pineGetCogOutput(PineCog* cog)
{
	return cog->out();
}

PINEAPI(void) pineRegisterFireCallback(PineCog* cog, CogFireCallback callback)
{
	cog->RegisterFireEventCallback(callback);
}

PINEAPI(PINERESULT) pineLoadBytecode(const char* path, CogBytecode* out)
{
	ifstream file(path, ios::binary | ios::in);
	if (!file.is_open())
	{
		return PINE_NOTFOUND;
	}
	UINT32 magic_c;
	
	file.read((char*)&magic_c, 4);

	if (magic_c != BYTECODE_MAGIC)
	{
		file.close();
		return PINE_FORMATERROR;
	}

	INT32 ptrcnt;
	file.read((char*)&ptrcnt, 4);

	INT32* ptrs = new INT32[ptrcnt];
	file.read((char*)ptrs, ptrcnt * 4);

	INT32 codelen;
	file.read((char*)&codelen, 4);

	byte* code = new byte[codelen];
	file.read((char*)code, codelen);
	
	file.close();

	CogBytecode* bc = new CogBytecode(code, ptrs, codelen, ptrcnt);
	*out = *bc;

	return PINE_OK;
}

PINEAPI(void) pineUnloadBytecode(CogBytecode* lpBytecode)
{
	delete lpBytecode;
	lpBytecode = nullptr;
}