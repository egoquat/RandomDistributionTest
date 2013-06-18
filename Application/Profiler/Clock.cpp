#include ".\clock.h"

CClock::CClock()
{
	Clear();
}

CClock::~CClock()
{

}

void CClock::Clear( void)
{
	m_bCustomTickMeasured = FALSE;
	m_llCustomTickPerMillisecond = 1000;

	m_llLatestFrameTick = 0;
	m_dwLatestFrameMoveTick = 0;
	m_dwLatestRenderTick = 0;
	m_iFramePerSecond = 40;
	m_bEnableRenderOnly = TRUE;
	m_lFrameCount = 0;
	m_iMinFramePerSecond = 10;
	m_bFixedFrame = TRUE;
	m_dwBaseTick = 0;
	m_bUpdateClock = FALSE;
}

void CClock::BeginMeasureTick( void)
{
	// ���۽ð� ���
	/*DWORD dwTrashTick = ::GetTickCount();
	while ( dwTrashTick == ( m_dwMeasureTick = ::GetTickCount()))
	{	// ��Ȯ�� ����� ���ؼ� Tick �� ���ŵ� ������ ���� ����Ѵ�.
	}*/
	m_llMeasureCustomTick = GetCustomTick();
}

void CClock::EndMeasureTick( void)
{
	// ���� �ð� ���
	LONGLONG llCurrentCustomTick = GetCustomTick();
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency( &liFrequency);
	m_llCustomTickPerMillisecond = liFrequency.QuadPart / 1000;
	m_bCustomTickMeasured = TRUE;

	// �ֱ� �׸� ƽ�� ���� �ð�����
	m_llLatestFrameTick = llCurrentCustomTick;
	m_dwLatestFrameMoveTick = GetCurrentTick();
	m_dwBaseTick = GetCurrentTick();
}

void CClock::AutoMeasure( DWORD dwMillisecond)
{
	// 0.1 �� ���� ���� ( dwMillisecond ����)
	BeginMeasureTick();
	//Sleep( 100);
	EndMeasureTick();
}

BOOL CClock::SetFramePerSecond( int iMaxFramePerSecond, BOOL bEnableRenderOnly, 
							   int iMinFramePerSecond, BOOL bFixedFrame)
{
	assert( 0 < iMaxFramePerSecond && iMaxFramePerSecond < 1000);

	m_iFramePerSecond = iMaxFramePerSecond;
	m_iMinFramePerSecond = ( iMinFramePerSecond > 0) ? iMinFramePerSecond : iMaxFramePerSecond;
	m_bEnableRenderOnly = bEnableRenderOnly;
	m_bFixedFrame = bFixedFrame;
	return ( TRUE);
}

WORD CClock::CheckFrameSkipping( DWORD *pdwTick)
{
	m_bUpdateClock = TRUE;

	LONGLONG llTickPerFrame = m_llCustomTickPerMillisecond * 1000 / m_iFramePerSecond;
	::Sleep( 0);	// CPU ������ ���� ����

	LONGLONG llCurrentCustomTick = GetCustomTick();
	// �ε� ������ ���� ������ ������ Ŭ ���� ó��
	LONGLONG llElapsedTick = llCurrentCustomTick - m_llLatestFrameTick;
	if ( llElapsedTick > llTickPerFrame * 100)
	{
		llElapsedTick = llTickPerFrame;
		m_llLatestFrameTick = llCurrentCustomTick - llTickPerFrame;
	}
	// ���ο� �������ΰ�?
	if ( llElapsedTick >= llTickPerFrame)
	{
		//{
		// add 2006.01.16
		if( !m_bFixedFrame)
		{
			LONGLONG llMinTickPerFrame = m_llCustomTickPerMillisecond * 1000 / m_iMinFramePerSecond;
			m_llLatestFrameTick += ( llElapsedTick < llMinTickPerFrame) ? llElapsedTick : llMinTickPerFrame;
		}
		else
		{
			m_llLatestFrameTick += llTickPerFrame;
		}
		++m_lFrameCount;

		// DWORD ƽ ����
		DWORD dwCurrentTick = GetCurrentTick();
		*pdwTick = GetPassTick( dwCurrentTick, m_dwLatestFrameMoveTick) /*dwCurrentTick - m_dwLatestFrameMoveTick*/;
		m_dwLatestFrameMoveTick = dwCurrentTick;

		// �׸��⵵ �ϴ��� üũ
		if ( llCurrentCustomTick - m_llLatestFrameTick <= llTickPerFrame ||
			( dwCurrentTick - m_dwLatestRenderTick > 5000))
		{
			m_dwLatestRenderTick = dwCurrentTick;
			return ( CFS_MOVE | CFS_RENDER);
		}
		return ( CFS_MOVE);
	}

	if ( m_bEnableRenderOnly)
	{
		*pdwTick = 1;
		m_dwLatestRenderTick = GetCurrentTick();
		if ( m_lFrameCount == 0)
		{
			return ( CFS_MOVE | CFS_RENDER);
		}
		// �׸��⸸
		return ( CFS_RENDER);
	}

	return ( 0);
}

void CClock::Reset( void)
{
	m_llLatestFrameTick = GetCustomTick();
}