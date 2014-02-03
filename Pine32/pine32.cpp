#include "pine32.h"

#include <iostream>
#include <fstream>
#include<Windows.h>

using namespace std;

#define validate(ptr) if (!(ptr)) { return PINE_INVALID; }

PINEAPI(void) pineCreateDevice(PineDevice** ppDevice)
{
	*ppDevice = new PineDevice();
}

PINEAPI(PINERESULT) pineDestroyDevice(PineDevice *pDevice)
{
	validate(pDevice)
	delete pDevice;
	return PINE_OK;
}

PINEAPI(PINERESULT) pineStepDevice(PineDevice *pDevice)
{
	validate(pDevice)
	return pDevice->Step();
}

PINEAPI(BOOL) pineGetEnabled(PineDevice *pDevice)
{
	return pDevice->GetEnabled();
}

PINEAPI(PINERESULT) pineDisable(PineDevice *pDevice)
{
	validate(pDevice)
	pDevice->Disable();
	return PINE_OK;
}

PINEAPI(PINERESULT) pineEnable(PineDevice *pDevice)
{
	validate(pDevice)
	pDevice->Enable();
	return PINE_OK;
}

PINEAPI(PINERESULT) pineCreateCog(PineDevice* pDevice, CogBytecode* bytecode, UINT32 period, PineCog **hCog)
{	
	validate(pDevice && bytecode)
	if (period == 0)
	{
		return PINE_BADPARAM;
	}
	else if (bytecode == nullptr)
	{
		return PINE_BADPARAM;
	}
	int p = 0;
	PINERESULT result = pDevice->Allocate(p);
	if (result == PINE_OK)
	{
		PineCog* cog = new PineCog(pDevice, period, bytecode);
		*hCog = cog;
		pDevice->Assign(p, *hCog);
	}
	return result;
}

PINEAPI(PINERESULT) pineDestroyCog(PineDevice* pDevice, PineCog* cog)
{
	validate(pDevice && cog)
	return pDevice->DeallocateObj(cog);
}

PINEAPI(PINERESULT) pineSetCogRegister(PineCog* cog, UINT32 reg, double value)
{
	validate(cog)
	return cog->setreg(reg, value);
}

PINEAPI(PINERESULT) pineGetCogRegister(PineCog* cog, UINT32 reg, double* valueOut)
{
	validate(cog)
	return cog->getreg(reg, valueOut);
}

PINEAPI(PINERESULT) pineGetCogOutput(PineCog* cog, double* output)
{	
	validate(cog)
	*output = cog->out();
	return PINE_OK;
}

PINEAPI(PINERESULT) pineRegisterFireCallback(PineCog* cog, CogFireCallback callback)
{
	validate(cog)
	cog->RegisterFireEventCallback(callback);
	return PINE_OK;
}

PINEAPI(PINERESULT) pineLoadBytecode(const char* path, CogBytecode** ppCode)
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

	*ppCode = new CogBytecode(code, ptrs, codelen, ptrcnt);

	return PINE_OK;
}

PINEAPI(PINERESULT) pineUnloadBytecode(CogBytecode *lpbc)
{
	validate(lpbc)
	delete lpbc;
	return PINE_OK;
}