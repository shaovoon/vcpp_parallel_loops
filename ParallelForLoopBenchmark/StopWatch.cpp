#include "StdAfx.h"
#include "StopWatch.h"

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

StopWatch::StopWatch(void)
{
	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		// Error; application can't continue.
	}

	m_wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(m_wTimerRes); 
}


StopWatch::~StopWatch(void)
{
	timeEndPeriod(m_wTimerRes);
}

void StopWatch::Start()
{
	m_nStartTime = timeGetTime();
}

UINT StopWatch::Stop()
{
	UINT endTime = timeGetTime();

	return endTime - m_nStartTime;
}
