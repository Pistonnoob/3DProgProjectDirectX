#ifndef _SYSTEM_H_
#define _SYSTEM_H_
//We define Lean and Mean because it will speed up our application as we do
//not need the things it removes.
#define WIN32_LEAN_AND_MEAN_

#include <windows.h>

#include "InputHandler.h"
#include "GraphicsHandler.h"

class System
{
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputHandler* m_Input;
	GraphicsHandler* m_Graphics;

	bool Frame();
	void InitializeWindows(int& width, int& height);
	void ShutdownWindows();
public:
	System();
	System(const System& orig);
	~System();

	bool Initialize();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

//Bellow: Used to redirect the windows system messaging into the
//MessageHandler funciton inside this class.

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

static System* ApplicationHandle = 0;

#endif