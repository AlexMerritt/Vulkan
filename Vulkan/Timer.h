#pragma once

#include <Windows.h>

class Timer
{
public:
	bool Initialize();
	void Update();
	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

