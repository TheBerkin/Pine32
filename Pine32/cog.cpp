#include<cmath>
#include<Windows.h>
#include <iostream>
#include <random>
#include <ctime>

#include "cog.h"
#include "cogmem.h"
#include "device.h"
#include "bytecode.h"
#include "opcodes.h"

using namespace std;

#define ranged_case(c) case c: if (!ranged) { break; }	
#define ranged_case_readint(c, reg) case c: if (!read_int(reg)) { return PINE_ENDOFSTREAM; } if (!ranged) { break; }
#define ranged_case_readint2(c, rega, regb) case c: if (!read_int(rega)) { return PINE_ENDOFSTREAM; } else if (!read_int(regb)) { return PINE_ENDOFSTREAM; } if (!ranged) { break; }
#define ranged_case_readdouble(c, v)  case c: if (!read_double(v)) { return PINE_ENDOFSTREAM; } if (!ranged) { break; }
#define case_readdouble2(c, v1, v2) case c: if (!read_double(v1)) { return PINE_ENDOFSTREAM; } else if (!read_double(v2)) { return PINE_ENDOFSTREAM; }
#define pop(x) if (_device->Pop(&x) != PINE_OK) { return PINE_STACKERROR; }
#define push(x) if (_device->Push(x) != PINE_OK) { return PINE_STACKERROR; }
#define ptr(r) if (!_code->ptr(r, &_readpos)) { return PINE_PTRERROR; }

cogptr::cogptr(int vi)
{
	i = vi;
}

cogptr::cogptr(double vd)
{
	d = vd;
}

PineCog::PineCog(PineDevice *lpDevice, UINT32 period, CogBytecode* bytecode)
{	
	_device = lpDevice;
	_code = bytecode;
	_registers = new double[NUM_REGISTERS];
	_cbFire = nullptr;
	_output = 0;
	_period = period;
	srand(time(0));

	rmin = 0.0;
	rmax = 1.0;
}

double PineCog::out()
{
	return _output;
}

PINERESULT PineCog::Iterate()
{	
	_readpos = 0;
	rmin = 0.0;
	rmax = 1.0;	
	x = double(_device->ticks() % _period) / _period;
	
	while (!eos())
	{		
		read_byte(ci);
		ranged = (x >= rmin && x <= rmax);
		
		switch (ci)
		{
			ranged_case(OP_ABS)
				pop(va);
				push(fabs(va));
			break;
			ranged_case(OP_ADD)
				pop(va);
				pop(vb);
				push(va + vb);
			break;
			ranged_case(OP_ATAN)
				pop(va);
				push(atan(va));
			break;
			ranged_case_readint(OP_CALL, r0)
				push(cogptr(_readpos).d);
				ptr(r0);
			break;
			ranged_case(OP_CEIL)
				pop(va);
				push(ceil(va));
			break;
			ranged_case(OP_CLAMP)
				pop(vb);
				pop(va);
				pop(vc);
				push(vc < va ? va : vc > vb ? vb : vc);
			break;
			ranged_case(OP_COPY)
				pop(va);
				push(va);
				push(va);
			break;
			ranged_case(OP_COS)
				pop(va);
				push(cos(va));
			break;
			ranged_case(OP_DIV)
				pop(vb);
				pop(va);
				push(vb == 0 ? 0 : va / vb);
			break;
			ranged_case(OP_FLOOR)
				pop(va);
				push(floor(va));
			break;
			ranged_case(OP_GE)
				pop(vb);
				pop(va);
				push(va >= vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_GT)
				pop(vb);
				pop(va);
				push(va > vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_LE)
				pop(vb);
				pop(va);
				push(va <= vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_LT)
				pop(vb);
				pop(va);
				push(va < vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_EQ)
				pop(vb);
				pop(va);
				push(va == vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_NE)
				pop(vb);
				pop(va);
				push(va != vb ? 1.0 : 0.0);
			break;
			ranged_case(OP_FIRE)
				fire();
			break;
			ranged_case(OP_ILERP)
				pop(vc);
				pop(vb);
				pop(va);
				push((vc - va) / (vb - va));
			break;
			ranged_case_readint2(OP_JALT, r0, r1)
				pop(va);
				if (va == 0)
				{
					ptr(r0);
				}
				else
				{
					ptr(r1);
				}
			break;
			ranged_case_readint(OP_JMP, r0)
				ptr(r0);
			break;
			ranged_case_readint(OP_JNZ, r0)
				pop(va);
				if (va != 0)
				{
					ptr(r0);
				}
			break;
			ranged_case(OP_LERP)
				pop(vb);
				pop(va);
				pop(vc);
				push(va + (vb - va) * vc);
			break;
			ranged_case(OP_MOD)
				pop(vb);
				pop(va);
				push(fmod(va, vb));
			break;
			ranged_case(OP_MUL)
				pop(vb);
				pop(va);
				push(va * vb);
			break;
			ranged_case(OP_NOT)
				pop(va);
				push(va == 0 ? 1.0 : 0.0);
			break;
			ranged_case_readint(OP_POP, r0)
				pop(va);
				if (setreg(r0, va) != PINE_OK)
				{
					return PINE_REGERROR;
				}
			break;
			ranged_case(OP_POW)
				pop(vb);
				pop(va);
				push(pow(va, vb));
			break;
			ranged_case_readint(OP_PRD, r0)
				pop(va);
				if (va <= 0)
				{
					return PINE_BADPARAM;
				}
				for (int i = 0; i < r0 - 1; i++)
				{
					pop(vb);
					va *= vb;
				}
				push(va);
			break;
			ranged_case_readint(OP_SUM, r0)
				pop(va);
				if (va <= 0)
				{
					return PINE_BADPARAM;
				}
				for (int i = 0; i < r0 - 1; i++)
				{
					pop(vb);
					va += vb;
				}
				push(va);
			break;
			ranged_case_readdouble(OP_PUSC, va)
				push(va);
			break;
			ranged_case(OP_PUSX)
				push(x);
			break;
			ranged_case(OP_STOP)
				return PINE_OK;
			ranged_case(OP_RET)
			{
				cogptr cp(0);
				cogptr* lpcp = &cp;
				pop(lpcp->d);
				ptr(lpcp->i);
			}
			break;
			ranged_case_readint(OP_PUSR, r0)
				if (getreg(r0, &va) != PINE_OK)
				{
					return PINE_REGERROR;
				}
				push(va);
			break;
			case_readdouble2(OP_RANGE, va, vb)
			if (vb < va)
			{
				rmin = 0.0;
				rmax = 1.0;
			}
			else
			{
				rmin = va;
				rmax = vb;
			}
			break;
			case OP_RANGEND:
				rmin = 0.0;
				rmax = 1.0;
			break;
			ranged_case(OP_SIN)
				pop(va);
				push(sin(va));
			break;
			ranged_case(OP_SQRT)
				pop(va);
				push(sqrt(va));
			break;
			ranged_case(OP_SUB)
				pop(vb);
				pop(va);
				push(va - vb);
			break;
			ranged_case(OP_TAN)
				pop(va);
				push(tan(va));
			break;
			ranged_case_readint(OP_ZERO, r0)
			if (setreg(r0, 0.0) != PINE_OK)
			{
				return PINE_REGERROR;
			}
			break;
			ranged_case(OP_RAND)
			{				
				pop(vb);
				pop(va);
				if (vb - va == 0.0 || va > vb)
				{
					push(0);
				}
				else
				{
					double rint = double(rand());
					double rfrac = fmod(cogptr(rand()).d, 1.0); // Convert the bits directly into a double
					push(fmod(rint + rfrac, vb - va) + va);
				}
			}
			break;
			ranged_case(OP_OUTS)
				pop(_output);
			break;
			ranged_case_readint(OP_OUTR, r0)
			if (getreg(r0, &_output) != PINE_OK)
			{
				return PINE_REGERROR;
			}
			break;
			ranged_case_readdouble(OP_OUTC, va)
				_output = va;
			break;
			default:
				return PINE_OPERROR;
		}
		
	}
	
	if (_registers[REGISTER_RESET] != 0.0)
	{
		ZeroMemory(_registers, NUM_REGISTERS * sizeof(double));
	}
	_output += _registers[REGISTER_OFFSET];
	return PINE_OK;
}

PINERESULT PineCog::getreg(UINT32 reg, double* out)
{
	if (reg < 0 || reg > NUM_REGISTERS)
	{
		return PINE_REGERROR;
	}
	*out = _registers[reg];
	return PINE_OK;
}

PINERESULT PineCog::setreg(UINT32 reg, double value)
{
	if (reg < 0 || reg > NUM_REGISTERS)
	{
		return PINE_REGERROR;
	}
	_registers[reg] = value;
	return PINE_OK;
}

BOOL PineCog::eos()
{
	return _readpos >= _code->length();
}

void PineCog::RegisterFireEventCallback(CogFireCallback cb)
{
	_cbFire = cb;	
}

void PineCog::fire()
{
	if (_cbFire != nullptr)
	{
		_cbFire(evdata());
	}
}

BOOL PineCog::read_byte(byte& out)
{
	if (_readpos + sizeof(byte) > _code->length())
	{
		_readpos = _code->length();
		return FALSE;
	}

	out = _code->GetByte(_readpos++);
	return TRUE;
}

BOOL PineCog::read_int(INT32& out)
{
	if (_readpos + sizeof(INT32) > _code->length())
	{
		_readpos = _code->length();
		return FALSE;
	}

	out = _code->GetInt32(_readpos);
	_readpos += sizeof(INT32);
	return TRUE;
}

BOOL PineCog::read_double(double& out)
{
	if (_readpos + sizeof(double) > _code->length())
	{
		_readpos = _code->length();
		return FALSE;
	}

	out = _code->GetDouble(_readpos);
	_readpos += sizeof(double);
	return TRUE;
}

EventRegisterData* PineCog::evdata()
{
	void *evd = malloc(sizeof EventRegisterData);
	memcpy(evd, _registers + REGISTER_EVENT_A, sizeof EventRegisterData);
	return (EventRegisterData*)evd;
}

PineCog::~PineCog()
{
	// Do NOT delete code. Multiple cogs can use the same code.
	delete[] _registers;
	_registers = nullptr;
	_device = nullptr;
}