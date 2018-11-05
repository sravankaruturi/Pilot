#include <cstdio>
#include "../Engine/TestScene.h"
#include "../Engine/Window.h"

int main(int _argc, char ** _argv)
{

	std::shared_ptr<Window> w = std::make_shared<Window>(1200, 800, "Game");
	pilot::TestScene * t = new pilot::TestScene(w);

	while(true)
	{
		
		t->RunScene();

	}

	return 0;
}