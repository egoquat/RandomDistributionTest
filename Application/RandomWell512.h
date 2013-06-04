#pragma once
#include <iostream>
#include <ctime>
#include <windows.h>

using namespace std;
class RandomWell512
{
protected:
	unsigned long states[16];
	UINT idxState;
	UINT SeedDiffer() { return 5 + (rand() % 500); }

public:
	RandomWell512()
	{
		//InitSeedStatic();
		InitSeedRandomize();
	}

	~RandomWell512(void){}

	void InitSeedStatic()
	{
		idxState = 0;
		UINT differ = SeedDiffer();
		UINT timecurrent = static_cast<UINT>(time(NULL));
		for(int i=0; i < 16; ++i)
		{
			states[i] = timecurrent + (i * differ);
		}
	}

	void InitSeedRandomize()
	{
		idxState = 0;
		UINT randseed = 0;
		for(int i=0; i < 16; ++i)
		{
			randseed = SeedDiffer();
			bool soleness = true;
			while(1)
			{
				soleness = true;
				for(int j = 0; j < i; ++j)
				{
					if(randseed == states[j])
					{
						soleness = false;
						break;
					}
				}
				if(false == soleness) { randseed = SeedDiffer(); } else {break;}
			}

			states[i] = randseed;
		}
	}

	UINT GetValue()
	{
		UINT a,b,c,d;

		a = states[idxState];
		c = states[(idxState + 13) & 15];
		b = a ^ c ^ (a << 16) ^ (c << 15);
		c = states[(idxState + 9) & 15];
		c = c ^ (c >> 11);
		a = states[idxState] = b ^ c;
		d = a ^ ((a << 15) & 0xda442d24U);
		idxState = (idxState + 15) & 15;
		a = states[idxState];
		states[idxState] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return states[idxState];
	}

	UINT GetValue(UINT minValue, UINT maxValue)
	{
		return minValue + (GetValue() % (maxValue - minValue));
	}

	UINT GetValue(UINT maxValue)
	{
		return GetValue() % maxValue;
	}

	float GetValueF()
	{
		UINT randValue = GetValue();
		
		union { unsigned long ul; float f; } p;
		p.ul = (((randValue = randValue * 16807) & 0x007fffff) - 1) | 0x3f800000;

		return p.f - 1.f;
	}

	float GetValueF(float minValue, float maxValue)
	{
		return minValue + ((maxValue - minValue) * GetValueF());
	}

	float GetValueF(float maxValue)
	{
		return GetValueF()* maxValue;
	}
};

