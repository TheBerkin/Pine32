#pragma once

#include <inttypes.h>
#include <Windows.h>

#define BYTECODE_MAGIC 0x50434243

class CogBytecode
{
private:
	byte* _code;
	INT32* _ptrs;
	INT32 _ptrcount;
	INT32 _length;

public:
	CogBytecode(byte*, INT32*, INT32, INT32);
	~CogBytecode();

	byte GetByte(INT32);
	INT32 GetInt32(INT32);
	UINT32 GetUInt32(INT32);
	double GetDouble(INT32);
	template<typename T>
	T Get(INT32);
	BOOL ptr(INT32, INT32*);
	
	INT32 length();
	INT32 pos();
};