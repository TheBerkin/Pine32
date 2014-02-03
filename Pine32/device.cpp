#include "device.h"
#include "cog.h"

PineDevice::PineDevice()
{
	_objects = new PineCog*[MAX_PINE_OBJECTS];
	for (int i = 0; i < MAX_PINE_OBJECTS; i++)
	{
		_objects[i] = nullptr;
	}
	_stack = new double[MAX_STACK_SIZE];
	_slotrange = 0;
	_stacksize = 0;
	_ticks = 0;
	Enabled = TRUE;
}

// Performs an iteration on each loaded cog.
PINERESULT PineDevice::Step()
{
	PINERESULT result = PINE_OK;
	if (!Enabled)
	{
		return result;
	}
	for (int i = 0; i < _slotrange; i++)
	{
		if (_objects[i] != nullptr)
		{
			result |= (_objects[i]->Iterate());
		}
	}
	_ticks++;
	return result;
}

void PineDevice::Enable()
{
	Enabled = TRUE;
}

void PineDevice::Disable()
{
	Enabled = FALSE;
}

BOOL PineDevice::GetEnabled()
{
	return Enabled;
}

UINT PineDevice::ticks()
{
	return _ticks;
}

// Allocates a cog and outputs the index at which it is allocated.
PINERESULT PineDevice::Allocate(int &lpCogIndex)
{
	int slot = nearest_empty();

	if (slot + 1 > _slotrange)
	{
		_slotrange = slot + 1;
	}
	else if (slot < 0)
	{
		return PINE_DEVICEFULL; // oh shit
	}	
	lpCogIndex = slot;
	return PINE_OK;
}

PINERESULT PineDevice::Assign(int index, PineCog *cog)
{
	if (_objects[index] != nullptr || index < 0 || index >= _slotrange || cog == nullptr)
	{
		return PINE_BADPARAM;
	}

	_objects[index] = cog;

	return PINE_OK;
}

// Deletes the cog at the specified index.
PINERESULT PineDevice::Deallocate(int cogIndex)
{
	if (cogIndex >= _slotrange || !_objects[cogIndex])
	{
		return PINE_EMPTYSLOT;
	}
	else
	{
		delete _objects[cogIndex];
		_objects[cogIndex] = nullptr;

		if (cogIndex + 1 == _slotrange)
		{
			do
			{
				_slotrange--;
				if (_slotrange == 0)
				{
					break;
				}
			} while (_objects[_slotrange - 1] == nullptr);
		}		
	}
	return PINE_OK;
}

// Returns the index of the specified cog.
int PineDevice::Find(PineCog* lpCog)
{
	for (int i = 0; i < _slotrange; i++)
	{
		if (_objects[i] == lpCog)
		{
			return i;
		}
	}
	return -1;
}

// Finds the specified cog and deletes it.
PINERESULT PineDevice::DeallocateObj(PineCog* lpCog)
{
	int index = Find(lpCog);
	if (index < 0)
	{
		return PINE_NOTFOUND;
	}
	else
	{
		return Deallocate(index);
	}
}

// Deletes all loaded cogs from memory.
void PineDevice::DeallocateAll()
{
	for (int i = 0; i < _slotrange; i++)
	{
		if (_objects[i])
		{
			delete _objects[i];
			_objects[i] = nullptr;
		}
	}
	_slotrange = 0;
}

// Finds the nearest empty cog slot.
int PineDevice::nearest_empty()
{
	for (int i = 0; i < MAX_PINE_OBJECTS; i++)
	{
		if (!_objects[i])
		{
			return i;
		}
	}
	return -1;
}

PINERESULT PineDevice::Push(double value)
{
	if (_stacksize == MAX_STACK_SIZE)
	{
		return PINE_STACKERROR;
	}
	
	_stack[_stacksize] = value;
	_stacksize++;
	return PINE_OK;
}

PINERESULT PineDevice::Pop(double* value)
{
	if (_stacksize == 0)
	{
		return PINE_STACKERROR;
	}
	*value = _stack[--_stacksize];
	return PINE_OK;
}

PineDevice::~PineDevice()
{
	DeallocateAll();
	delete[] _objects;
	_objects = nullptr;
	delete[] _stack;
	_stack = nullptr;
}