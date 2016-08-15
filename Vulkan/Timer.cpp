#include "Timer.h"

bool Timer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);

	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void Timer::Update()
{
	INT64 currentTime;
	float timeDifference;


	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	timeDifference = (float)(currentTime - m_startTime);

	// Calculate the frame time by the time difference over the timer speed resolution.
	m_frameTime = timeDifference / m_ticksPerMs;

	// Restart the timer.
	m_startTime = currentTime;

}

float Timer::GetTime()
{
	return m_frameTime;
}