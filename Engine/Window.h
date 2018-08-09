#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_KEYS		1024
#define MAX_BUTTONS		16

enum key_bool
{
	key_inactive,
	key_pressed,
	key_held,
	key_released
};

class Window
{
public:


	Window(unsigned width, unsigned height, const std::string& title);
	~Window();

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

	int width;

public:

	void SetWidth(unsigned _width)
	{
		width = _width;
	}

	void SetHeight(unsigned _height)
	{
		height = _height;
	}

	void UpdateFrameSize();

protected:
	int height;
	std::string title;

	bool isGlfwInit = false;
	bool isGladInit = false;

public:
	bool IsGladInit() const
	{
		return isGladInit;
	}

protected:
	GLFWwindow * window = nullptr;

public:
	GLFWwindow* GetWindow() const
	{
		return window;
	}

	bool IsGlfwInit() const
	{
		return isGlfwInit;
	}

	/**
	* \brief The keys and their current state.
	*/
	key_bool keys[MAX_KEYS]{};
	/**
	* \brief The Keys and their Previous State.
	*/
	key_bool prevKeys[MAX_KEYS]{};


	/**
	* \brief The Mouse buttons current state.
	*/
	key_bool mouseButtons[MAX_BUTTONS] = { key_inactive };
	/**
	* \brief The Mouse buttons Previous state.
	*/
	key_bool prevMouseButtons[MAX_BUTTONS] = { key_inactive };

	/**
	* \brief Mouse X Position in the Previous frame.
	*/
	double prevMouseX{ 0.0 };
	/**
	* \brief Mouse Y Position in the Previous frame.
	*/
	double prevMouseY{ 0.0 };
	/**
	* \brief Mouse X Position in the Current Frame.
	*/
	double mouseX{ 0.0 };
	/**
	* \brief Mouse Y Positin in the Current Frame.
	*/
	double mouseY{ 0.0 };
	/**
	* \brief Mouse X offset from the last frame.
	*/
	double mouseOffsetX{ 0.0 };
	/**
	* \brief Mouse Y offset from the last frame.
	*/
	double mouseOffsetY{ 0.0 };

	void HandleInput() const;


private:

	static void CleanUp();

};
