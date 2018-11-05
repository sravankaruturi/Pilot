#include <cstdio>
#include "../Engine/TestScene.h"
#include "../Engine/Window.h"

int main(int _argc, char ** _argv)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::shared_ptr<Window> w = std::make_shared<Window>(1200, 800, "Game");
	pilot::TestScene * t = new pilot::TestScene(w);

	while(!t->ShutDown())
	{
		
		t->RunScene();

	}

	delete t;

	return 0;
}