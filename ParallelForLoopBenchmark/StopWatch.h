#pragma once
class StopWatch
{
public:
	StopWatch(void);
	~StopWatch(void);

	void Start();
	UINT Stop();
private:
	UINT m_wTimerRes;

	UINT m_nStartTime;

};

