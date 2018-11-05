#define Testing 1

#if Testing
#include "../Engine/Animation.h"
int main(int _argc, char ** _argv)
{
	pilot::Animation a = pilot::Animation("Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Models/RTSDemo/animations/WK_heavy_infantry_04_charge.FBX");
}

#else

#include <cstdio>
#include "../Engine/TestScene.h"
#include "../Engine/Window.h"

int main(int _argc, char ** _argv)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::shared_ptr<Window> w = std::make_shared<Window>(1200, 800, "Game");
	std::unique_ptr<pilot::TestScene> t = std::make_unique<pilot::TestScene>(w);

	while(!t->ShutDown())
	{
		
		t->RunScene();

	}

	return 0;
}

#endif
