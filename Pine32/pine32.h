#pragma once

#include "device.h"
#include "bytecode.h"
#include "cog.h"
#include <Windows.h>

// API shit
#define PINEAPI(ret_type) __declspec(dllexport) ret_type __stdcall

extern "C"
{
	PINEAPI(void) pineCreateDevice(PineDevice** ppDevice);
	PINEAPI(PINERESULT) pineDestroyDevice(PineDevice* device);
	PINEAPI(PINERESULT) pineStepDevice(PineDevice* device);
	PINEAPI(BOOL) pineGetEnabled(PineDevice* device);
	PINEAPI(PINERESULT) pineDisable(PineDevice* device);
	PINEAPI(PINERESULT) pineEnable(PineDevice* device);

	PINEAPI(PINERESULT) pineCreateCog(PineDevice* device, CogBytecode* code, UINT32 period, PineCog **hCog);
	PINEAPI(PINERESULT) pineDestroyCog(PineDevice* device, PineCog*);
	PINEAPI(PINERESULT) pineSetCogRegister(PineCog* cog, UINT32 reg, double in);
	PINEAPI(PINERESULT) pineGetCogRegister(PineCog* cog, UINT32 reg, double* out);
	PINEAPI(PINERESULT) pineRegisterFireCallback(PineCog* cog, CogFireCallback cb);
	PINEAPI(PINERESULT) pineGetCogOutput(PineCog* cog, double* output);

	PINEAPI(PINERESULT) pineLoadBytecode(const char* path, CogBytecode** ppCode);
	PINEAPI(PINERESULT) pineUnloadBytecode(CogBytecode *code);
}