#include "System.h"



System::System()
{
	m_Input = NULL;
	m_Graphics = NULL;
}

System::System(const System & orig)
{
}

System::~System()	//We free the memory in the shutdown function as the destructor for a windows class aren't always called
{
}

bool System::Initialize()
{
	bool result = true;
	int screenWidth, screenHeight;

	//Initialize the height and width of the screen to zero
	screenWidth = 0;
	screenHeight = 0;
	//Initialize the windows api
	this->InitializeWindows(screenWidth, screenHeight);

	//Create the input object. Will be used to handle reading keyboard input.
	m_Input = new InputHandler();
	if (!m_Input)
	{
		return false;
	}
	//Initialize the input object
	m_Input->Initialize();

	//Create the graphics object. This object will handle rendering the graphics for the application.
	m_Graphics = new GraphicsHandler();
	if (!m_Graphics)
	{
		return false;
	}

	//Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}


	return result;
}

void System::ShutDown()
{
	//Release the graphics object
	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	//Release the input object
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	//Shutdown the window
	this->ShutdownWindows();

	return;
}

void System::Run()
{
	//This function is where our application will lood until we decide to stop.
	//The application processing is done in the Frame function which is called each loop.

	MSG msg;
	bool done = false, result = false;

	//Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));	//memset

	//Loop until there is a quit message.
	while (!done)
	{
		//Handle the windows messages/events
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//Check if a key has been pressed.
		case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		//Check if a key has been released
		case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		//Any other messages send to the default mesage handler as we won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}


bool System::Frame()
{
	bool result = true;

	//Check if the user pressed escape and wants to exit the application,
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//Do the frame processing for the graphics object.
	result = m_Graphics->Frame();	
	if (!result)
	{
		return false;
	}

	return result;
}

void System::InitializeWindows(int & width, int & height)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX = 0, posY = 0;
	
	//get an external pointer to this object.
	ApplicationHandle = this;

	//Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	//Give the application a name.
	m_applicationName = L"Engine";

	//Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop screen.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen setting depending on whether it is running in ful screen or in windowed mode
	if (FULL_SCREEN)
	{
		//If fill screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		//If windowed mode set it to 800x600 resolution
		width = 800;
		height = 600;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, width, height, NULL, NULL, m_hinstance, NULL);

	//Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide the mouse cursor
	ShowCursor(true);

	return;
}

//ShutdownWindows does just that. It returns the screen settings back to normal and releases the window and the handles associated with it.
void System::ShutdownWindows()
{
	//Show the mouse cursor.
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}
