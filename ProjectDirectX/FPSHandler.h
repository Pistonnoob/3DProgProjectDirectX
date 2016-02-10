#ifndef _FPSHANDLER_H_
#define _FPSHANDLER_H_
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
class FPSHandler
{
	int m_FPS, m_count;
	unsigned long m_start;
public:
	FPSHandler();
	FPSHandler(const FPSHandler& original);
	~FPSHandler();

	void Initialize();
	void Frame();
	int GetFps();
private:
};

#endif
