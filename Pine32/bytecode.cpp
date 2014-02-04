#include "bytecode.h"
#include<Windows.h>

CogBytecode::CogBytecode(byte* data, INT32* ptrs, INT32 length, INT32 ptrcount)
{
	_code = data;
	_ptrs = ptrs;
	_length = length;
	_ptrcount = ptrcount;
}

INT32 CogBytecode::length()
{
	return _length;
}

byte CogBytecode::GetByte(INT32 pos)
{
	return _code[pos];
}

INT32 CogBytecode::GetInt32(INT32 pos)
{
	return *((INT32*)(_code + pos));
}

DOUBLE CogBytecode::GetDouble(INT32 pos)
{
	return *((DOUBLE*)(_code + pos));
}

UINT32 CogBytecode::GetUInt32(INT32 pos)
{
	return *((UINT32*)(_code + pos));
}

template <typename T>
T CogBytecode::Get(INT32)
{
	return *((T*)(_code + pos));
}

BOOL CogBytecode::ptr(INT32 index, INT32* ptr)
{
	if (index >= _ptrcount)
	{
		*ptr = 0;
		return false;
	}
	*ptr = _ptrs[index];
	return true;
}

CogBytecode::~CogBytecode()
{
	delete[] _code;
	delete[] _ptrs;
}