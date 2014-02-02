#include "bytecode.h"
#include<Windows.h>

CogBytecode::CogBytecode(byte* data, INT32* ptrs, INT32 length, INT32 ptrcount)
{
	this->_code = data;
	this->_ptrs = ptrs;
	this->_length = length;
	this->_ptrcount = ptrcount;
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
	return *((INT32*)(&this->_code + pos));
}

DOUBLE CogBytecode::GetDouble(INT32 pos)
{
	return *((DOUBLE*)(&this->_code + pos));
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
}