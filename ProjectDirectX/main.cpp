#include "System.h"
#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::string resLoc = setlocale(LC_CTYPE, "");
	CoInitialize(NULL);
	System* system = NULL;
	bool result = false;

	//Create the system object.
	system = new System();
	if (!system)
	{
		return 0;
	}

	//Initialize and run the system object.
	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	//Shutdown and release the system object
	system->ShutDown();
	delete system;
	system = NULL;

	return 0;
}