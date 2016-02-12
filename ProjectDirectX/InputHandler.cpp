#include "InputHandler.h"



InputHandler::InputHandler()
{
	this->m_directInput = NULL;
	this->m_keyboard = NULL;
	this->m_mouse = NULL;
}

InputHandler::InputHandler(const InputHandler & input)
{
}


InputHandler::~InputHandler()
{
}

bool InputHandler::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	HRESULT result;
	this->m_screenWidth = width;
	this->m_screenHeight = height;
	
	this->m_mouseX = 0;
	this->m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputHandler::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputHandler::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = this->ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = this->ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	this->ProcessInput();

	return true;
}

bool InputHandler::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (this->m_keys[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputHandler::IsKeyPressed(int keyboardScanCode)
{
	bool result = false;
	if (keyboardScanCode < sizeof(this->m_keys) && keyboardScanCode > 0)
	{
		if (this->m_keys[keyboardScanCode])
		{
			return true;
		}
	}

	return result;
}

void InputHandler::GetMouse(int & x, int & y)
{
	x = this->m_mouseX;
	y = this->m_mouseY;
	return;
}

bool InputHandler::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = this->m_keyboard->GetDeviceState(sizeof(this->m_keys), (LPVOID)&this->m_keys);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			this->m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputHandler::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputHandler::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}
