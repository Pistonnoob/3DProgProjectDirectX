#include "Timer.h"



Timer::Timer()
{
	this->m_frequency = 0;
	this->m_ticksPerMs = 0;
	this->m_startTime = 0;
}

Timer::Timer(const Timer & original)
{
	this->m_frequency = original.m_frequency;
	this->m_ticksPerMs = original.m_ticksPerMs;
	this->m_startTime = original.m_startTime;
	this->m_frameTime = original.m_frameTime;
}

Timer::~Timer()
{
}


bool Timer::Initialize()
{
	//Check to see if this system supports high frequency timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
		return false;
	//Store how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	return true;
}

void Timer::Frame()
{
	INT64 curTime = 0;
	float deltaTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	
	deltaTime = (float)(curTime - m_startTime);
	//Calculate the time frame time.
	m_frameTime = deltaTime / m_ticksPerMs;
	//Update the start time to the current time.
	m_startTime = curTime;

	return;
}

float Timer::GetTime()
{
	return this->m_frameTime;
}
