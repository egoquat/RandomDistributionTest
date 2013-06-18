#pragma once

#include <WTypes.h>
#include <windows.h>
#include <stdio.h>
#include "CommonUtil_.h"

#define MAX_PROFILESAMPLENAME	( 64)
#define MAX_PROFILERESULT		( 512)

class CProfileUnit  
{
public:
	CProfileUnit();
	CProfileUnit( unsigned int uikey, const char *szProfileUnitName );
	~CProfileUnit();
	void Clear( void);

public:
	unsigned int key;

protected:
	char _txtNameUnit[MAX_PROFILESAMPLENAME];
	char _txtOutputUnit[MAX_PROFILERESULT];

	/// @name 계산 정보
	LONGLONG m_llStartTick;			///< 한번 체크하는 동안의 시작
	LONGLONG m_llSumTime;			///< 한 프레임 동안 총 합
	LONGLONG m_llSumTime_GrandTotal;	///< 현재까지의 누적치 총 합

	/// @name 차지하는 %
	float m_fLatestPercentage;
	float m_fMaxPercentage;
	float m_fMinPercentage;
	float m_fTotalPercentage;

	float m_fAve_ms;			// 한 프레임동안 호출시간평균
	float m_fAve_ms_GrandTotal;

	unsigned long m_ulAve_tick;
	unsigned long m_ulAve_tick_GrandTotal;

	/// @name 호출 회수
	unsigned long m_ulCountInFrame;		///< 한 프레임 동안 호출된 회수
	unsigned long m_ulCountInFrame_GrandTotal;	// 현재까지의 총 호출된 회수

	unsigned long m_ulLatestCount;		///< 최근의 호출 회수

private:
	void Init( unsigned int uikey, const char *lpszName);

public:
	BOOL IsCheckNameProfileUnit( char *lpszName);
	char* GetNameProfileUnit( void)	{ return ( _txtNameUnit); }

	void BeginUnit_( unsigned int uiKey, const char *lpszName, LONGLONG llTick );
	void EndUnit_( LONGLONG llTick);	

	void CalculateResult( LONGLONG llTotalTime, LONGLONG llGrandTotalTime, LONGLONG llTickPerMS = 1 );

	void GetResult_Percentage( float *pfLatestPercentage, float *pfMaxPercentage, float *pfMinPercentage, float *pfTotalPercentage);
	void GetResult_StatisticAve( float *pfAve_ms, 
							unsigned long *pulAve_tick, 
							float *pfAve_ms_GrandTotal,
							unsigned long *pulAve_tick_GrandTotal );

	float ResultProfileMSTime() 
	{ 
		return m_fAve_ms_GrandTotal; 
	}
	void OutputDebugResultText();
	void ResultText( char *szTextResult );
	void ResultText_Avg( char *szTextResult );
};
