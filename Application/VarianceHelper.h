#pragma once
#include "Common.h"

template <typename T>
class CVarianceHelper
{
	int _countData;
	T* _dataTable;

	T _average;
	T _variance;
	T _standardDeviation;

public:
	T getAverage(){ return _average; }
	T getVariance() { return _variance; }
	T getStandardDeviation() { return _standardDeviation; }
	UINT getCountData(const T data)
	{
		if(NULL==_dataTable || 1 > _countData) { assert(0 && "NULL==_dataTable || 1 > _countData"); return 0;}
		UINT count = 0;
		for(int i=0; i<_countData; ++i)
		{
			if(data == _dataTable[i]){++count;}
		}
		return count;
	}

	void Initialize(int countOfData)
	{
		_countData = countOfData;
		_dataTable = new T[countOfData];
		memset(_dataTable,0, sizeof(T) * countOfData);
	}

	void SetData(int idx, T data)
	{
		if(idx < 0 || idx >= _countData) { assert(0 && "idx < 0 || idx >= _countData"); return;}
		
		_dataTable[idx] = data;
	}

	void Calculate()
	{
		if(NULL==_dataTable || 1 > _countData) { assert(0 && "NULL==_dataTable || 1 > _countData"); return;}

		T summary = 0;

		for(int i=0; i<_countData; ++i)
		{
			summary = summary + _dataTable[i];
		}

		_average = summary / _countData;

		T differSum = 0;
		for(int i=0; i<_countData; ++i)
		{
			T diff = (_dataTable[i] - _average);
			differSum = differSum + (diff * diff);
		}

		_variance = differSum / _countData;
		_standardDeviation = sqrt(static_cast<double>(_variance));
	}

	void Release()
	{
		SAFEDELETE_ARRAY(_dataTable);
		_dataTable = NULL;
		_countData = 0;
		_average = 0;
		_variance = 0;
		_standardDeviation = 0;
	}

	CVarianceHelper()
	{
		_dataTable = NULL;
		_countData = 0;
		_average = 0;
		_variance = 0;
		_standardDeviation = 0;
	}
};

