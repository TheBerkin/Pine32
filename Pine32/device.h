#pragma once

#include <Windows.h>

// Constants
#define MAX_PINE_OBJECTS 2048
#define MAX_STACK_SIZE 512

// PINE result codes
typedef DWORD PINERESULT;

#define PINE_OK				0x00000000 // All is good.
#define PINE_STACKERROR		0x00000001 // Generic stack error. Can refer to overflow, imbalance or popping from empty stack.
#define PINE_PTRERROR		0x00000002 // Bad jump point, usually indicates a compiler issue.
#define PINE_FORMATERROR	0x00000004 // Your bytecode is corrupt probably.
#define PINE_NOTFOUND		0x00000008 // Cog couldn't be found in the cache.
#define PINE_DEVICEFULL		0x00000010 // Device can't allocate cog because it's full.
#define PINE_EMPTYSLOT		0x00000020 // Tried to access an empty cog slot.
#define PINE_ENDOFSTREAM	0x00000040 // Tried to read past the end of the stream.
#define PINE_REGERROR		0x00000080 // Tried to access an invalid register.
#define PINE_BADPARAM		0x00000100 // Tried to pass a horribly wrong parameter.
#define PINE_OPERROR		0x00000200 // Encountered an unrecognized instruction.
#define PINE_INVALID		0x00000400 // Tried to use a bad pointer.


class PineCog;

class PineDevice
{
public:
	PineDevice();
	~PineDevice();

	void Enable();
	void Disable();
	int GetEnabled();

	PINERESULT Step();
	PINERESULT Push(double);
	PINERESULT Pop(double*);

	PINERESULT Allocate(int&);
	PINERESULT Assign(int, PineCog*);
	PINERESULT Deallocate(int);
	PINERESULT DeallocateObj(PineCog*);
	void DeallocateAll();
	int Find(PineCog*);
	
	UINT ticks();
	
private:
	BOOL Enabled;
	UINT _ticks;
	PineCog** _objects; // MAX_PINE_OBJECTS
	int _slotrange;
	int nearest_empty();

	double* _stack; // MAX_STACK_SIZE
	int _stacksize;
};