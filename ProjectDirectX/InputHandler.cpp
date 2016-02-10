#include "InputHandler.h"



InputHandler::InputHandler()
{
}

InputHandler::InputHandler(const InputHandler & input)
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::Initialize()
{
	int i = 1;
	//Initialize all keys as released/not pressed
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return;
}

void InputHandler::KeyDown(unsigned int key)
{
	m_keys[key] = true;
	return;
}

void InputHandler::KeyUp(unsigned int key)
{
	m_keys[key] = false;
}

bool InputHandler::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
