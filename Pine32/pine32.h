#pragma once

#include "device.h"
#include "bytecode.h"
#include "cog.h"
#include <Windows.h>

// API shit
#define PINEAPI(ret_type) __declspec(dllexport) ret_type __stdcall

extern "C"
{
	PINEAPI(PineDevice*) pineCreateDevice();
	PINEAPI(void) pineDestroyDevice(PineDevice* device);
	PINEAPI(PINERESULT) pineStepDevice(PineDevice* device);
	PINEAPI(BOOL) pineGetEnabled(PineDevice* device);
	PINEAPI(void) pineDisable(PineDevice* device);
	PINEAPI(void) pineEnable(PineDevice* device);

	PINEAPI(PINERESULT) pineCreateCog(PineDevice* device, CogBytecode* code, int period, PineCog* cog);
	PINEAPI(PINERESULT) pineDestroyCog(PineDevice* device, PineCog*);
	PINEAPI(PINERESULT) pineSetCogRegister(PineCog* cog, UINT32 reg, double in);
	PINEAPI(PINERESULT) pineGetCogRegister(PineCog* cog, UINT32 reg, double* out);
	PINEAPI(void) pineRegisterFireCallback(PineCog* cog, CogFireCallback cb);
	PINEAPI(double) pineGetCogOutput(PineCog* cog);

	PINEAPI(PINERESULT) pineLoadBytecode(const char* path, CogBytecode* out);
	PINEAPI(void) pineUnloadBytecode(CogBytecode* code);
}