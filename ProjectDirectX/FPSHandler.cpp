#include "FPSHandler.h"



FPSHandler::FPSHandler()
{
	m_FPS = 0;
	m_count = 0;
	m_start = 0;
}

FPSHandler::FPSHandler(const FPSHandler & original)
{
	m_FPS = original.m_FPS;
	m_count = original.m_count;
	m_start = original.m_start;
}


FPSHandler::~FPSHandler()
{
}

void FPSHandler::Initialize()
{
	this->m_start = timeGetTime();
}

void FPSHandler::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_start + 1000))
	{
		m_FPS = m_count;
		m_count = 0;

		m_start = timeGetTime();
	}
}

int FPSHandler::GetFps()
{
	return m_FPS;
}
