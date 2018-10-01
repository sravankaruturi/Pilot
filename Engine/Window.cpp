#include "Window.h"
#include <iostream>

#include "Configurations.h"

#include "../EngineDeps/external_files/ImGUI/imgui.h"
#include "../EngineDeps/external_files/ImGUI/imgui_impl_glfw.h"

void window_resize(GLFWwindow * _window, int _width, int _height);
void key_callback(GLFWwindow * _window, int _key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods);
void cursor_position_callback(GLFWwindow* _window, double _xpos, double _ypos);

Window::Window(unsigned _width, unsigned _height, const std::string& _title): width(_width),
                                                                           height(_height),
                                                                           title(_title)
{

	isGlfwInit = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if _APPLE
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (nullptr == window)
	{
		CleanUp();
	}

	glfwMakeContextCurrent(this->window);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_resize);
	glfwSetCursorPosCallback(window, cursor_position_callback);

#if ENABLE_GUI

	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	// TODO: What to do when there is no GUI?

#endif


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		isGladInit = false;
		std::cout << "Failed to initialize GLAD" << std::endl;
	}else
	{
		isGladInit = true;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);

}

Window::~Window()
{
	CleanUp();
}

unsigned Window::GetWidth()
{
	glfwGetFramebufferSize(window, &this->width, &this->height);
	return width;
}

unsigned Window::GetHeight()
{
	glfwGetFramebufferSize(window, &this->width, &this->height);
	return height;
}

std::string Window::GetTitle() const
{
	return title;
}

void Window::SetWidth(unsigned _width)
{
	width = _width;
}

void Window::SetHeight(unsigned _height)
{
	height = _height;
}

bool Window::IsGladInit() const
{
	return isGladInit;
}

GLFWwindow* Window::GetWindow() const
{
	return window;
}

bool Window::IsGlfwInit() const
{
	return isGlfwInit;
}

bool Window::IsKeyPressed(unsigned _keyCode) const
{
	// TODO: Log this.
	if (_keyCode >= MAX_KEYS)
	{
		return false;
	}

	return (keys[_keyCode] == key_pressed);
}

bool Window::IsKeyPressedAndReleased(unsigned _keyCode) const
{
	// TODO: Log this.
	if (_keyCode >= MAX_KEYS)
	{
		return false;
	}

	return (keys[_keyCode] == key_released);
}

bool Window::IsKeyPressedOrHeld(const unsigned _keycode) const
{
	return IsKeyPressed(_keycode) || IsKeyHeld(_keycode);
}

bool Window::IsKeyHeld(unsigned _keyCode) const
{
	return (_keyCode >= MAX_KEYS) ? false : (keys[_keyCode] == key_held);
}

bool Window::IsMouseButtonPressed(unsigned _button) const
{
	// TODO: Log this.
	if (_button >= MAX_BUTTONS)
	{
		return false;
	}

	return (mouseButtons[_button] == key_pressed);
}

void Window::GetMousePosition(double& x, double& y) const
{
	x = mouseX;
	y = mouseY;
}

void Window::UpdateFrameSize()
{
	glfwGetFramebufferSize(this->window, &this->width, &this->height);
}

void Window::HandleInput() const
{

	if ( IsKeyPressedAndReleased(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

}

void Window::Update(const float _deltatime)
{
	// Swap Buffers
	glfwSwapBuffers(window);

	/**/
	// పాతవన్నీ ఒకచోట దాచుకొని మళ్ళీ చూడు
	for (auto i = 0; i < MAX_KEYS; i++)
	{
		prevKeys[i] = keys[i];
	}
	for (auto i = 0; i < MAX_BUTTONS; i++)
	{
		prevMouseButtons[i] = mouseButtons[i];
	}

	prevMouseX = mouseX;
	prevMouseY = mouseY;

	// Poll events
	glfwPollEvents();

	for (auto i = 0; i < MAX_KEYS; i++)
	{
		if ((prevKeys[i] == key_released) && (keys[i] == key_released))
		{
			keys[i] = key_inactive;
		}
	}

	for (auto i = 0; i < MAX_BUTTONS; i++)
	{
		if ((prevMouseButtons[i] == key_released) && (mouseButtons[i] == key_released))
		{
			mouseButtons[i] = key_inactive;
		}
	}

	/* This banks on the fact that to get to 0.0 and 0.0 again would be pretty hard. */
	if (prevMouseX + prevMouseY < std::numeric_limits<float>::epsilon())
	{
		mouseOffsetX = 0;
		mouseOffsetY = 0;
	}
	else
	{
		mouseOffsetX = mouseX - prevMouseX;
		// Since the MouseCursor Pos in OpenGl is Inverted.
		mouseOffsetY = -1 * (mouseY - prevMouseY);
	}

	auto error = glGetError();
}

void Window::CleanUp()
{
	glfwTerminate();
}

void window_resize(GLFWwindow * _window, int _width, int _height) {

	auto win = static_cast<Window *>(glfwGetWindowUserPointer(_window));

	// The Functions GetWidth and GetHeight give the udpated stuff.
	win->UpdateFrameSize();

	//glViewport(0, 0, win->GetWidth(), win->GetHeight());
}

void key_callback(GLFWwindow * _window, int _key, int scancode, int action, int mods) {

#if ENABLE_GUI
	if (ImGui::GetIO().WantCaptureKeyboard) {
		ImGui_ImplGlfw_KeyCallback(_window, _key, scancode, action, mods);
	}
	else {

#endif

		auto win = static_cast<Window *>(glfwGetWindowUserPointer(_window));


		// ఇక్కడ మనం ప్రస్తుతమున్న State బట్టి కొత్త state పెడతాం
		if (win->keys[_key] == key_pressed || win->keys[_key] == key_held)
		{
			if (action == GLFW_RELEASE)
			{
				win->keys[_key] = key_released;
			}
			else if (action == GLFW_REPEAT)
			{
				win->keys[_key] = key_held;
			}
		}
		else
		{
			if (action == GLFW_PRESS)
			{
				win->keys[_key] = key_pressed;
			}
		}

#if ENABLE_GUI
	}
#endif

}

void mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods) {

#if ENABLE_GUI
	if (ImGui::GetIO().WantCaptureMouse) {
		return;
	}
#endif

	auto * win = (Window *)glfwGetWindowUserPointer(_window);

	// ఇక్కడ మనం ప్రస్తుతమున్న State బట్టి కొత్త state పెడతాం

	if (GLFW_PRESS == _action)
	{
		win->mouseButtons[_button] = key_pressed;
	}
	else if (GLFW_RELEASE == _action)
	{
		win->mouseButtons[_button] = key_released;
	}
	else
	{
		/* This shouldn't happen */
		//s_debug.Print("---------------------------------------------------------------------------");
		//s_debug.Print("Mouse buttons received an action that is different from Pressed or Released");
		//s_debug.Print("---------------------------------------------------------------------------");
		win->mouseButtons[_button] = key_inactive;
	}

}

/**
* \brief
* \param _window
* \param _xpos
* \param _ypos
*/
void cursor_position_callback(GLFWwindow* _window, double _xpos, double _ypos) {

	auto win = static_cast<Window *>(glfwGetWindowUserPointer(_window));

	win->mouseX = _xpos;
	win->mouseY = _ypos;

	//std::cout << win->mouseX << ", " << win->mouseY << std::endl;

}