#pragma once
#include <string>

class Window
{
public:


	Window(unsigned width, unsigned height, const std::string& title)
		: width(width),
		  height(height),
		  title(title)
	{
	}


	unsigned GetWidth() const
	{
		return width;
	}

	unsigned GetHeight() const
	{
		return height;
	}

	std::string GetTitle() const
	{
		return title;
	}

protected:

	unsigned int width;
	unsigned int height;
	std::string title;


};
