#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

class InputHandler
{
private:
	bool m_keys[256];
public:
	InputHandler();
	InputHandler(const InputHandler& input);
	~InputHandler();

	void Initialize();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	
	bool IsKeyDown(unsigned int key);
};

#endif