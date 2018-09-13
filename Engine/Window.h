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

/**
 * \brief Window Class. Initialize this for opening the window. Cleanup to exit.
 */
class Window
{

protected:

	/**
	 * \brief Width of the Window in Pixels.
	 */
	int width;

	/**
	 * \brief Height of the Window in Pixels.
	 */
	int height;

	/**
	 * \brief Title of the Window.
	 */
	std::string title;

	/**
	 * \brief Flag to check if GLFW is Initialized.
	 */
	bool isGlfwInit = false;

	/**
	 * \brief Flag to check if Glad is Initialized.
	 */
	bool isGladInit = false;

	/**
	 * \brief The actual GLFW Window Pointer. Nullptr if GLFW is not initialized.
	 */
	GLFWwindow * window = nullptr;

public:

	/**
	 * \brief Creates the Window and Opens it.
	 * \param _width Width of the Window
	 * \param _height Height of the Window
	 * \param _title Title of the Window
	 */
	Window(unsigned _width, unsigned _height, const std::string& _title);

	/**
	 * \brief Default Destructor. Calls Cleanup.
	 */
	~Window();

	/** @defgroup Getters
	* Getters
	* @{
	*/

	unsigned GetWidth();

	unsigned GetHeight();

	std::string GetTitle() const;

	bool IsGladInit() const;

	bool IsGlfwInit() const;

	GLFWwindow* GetWindow() const;

	/*
	* @} // End of Getters
	*/

	/** @defgroup Setters
	* Setters
	* @{
	*/

	void SetWidth(unsigned _width);

	void SetHeight(unsigned _height);

	/* @}
	*/

	/**
	 * \brief Updates the Class's width and height with the GLFW Frame Buffer Width and Height.

		Depends on a GLFW Call.
	 */
	void UpdateFrameSize();


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

	bool IsKeyPressed(unsigned int _keyCode) const;

	bool IsKeyPressedAndReleased(unsigned int _keyCode) const;

	bool IsKeyPressedOrHeld(const unsigned int _keycode) const;

	bool IsKeyHeld(unsigned int _keyCode) const;

	bool IsMouseButtonPressed(unsigned int _button) const;

	/* TODO: Implement this function later on */
	//bool IsMouseButtonPressedAndReleased(unsigned int _button) const
	//{
	//	// TODO: Log this.
	//	if (_button >= MAX_BUTTONS) {
	//		return false;
	//	}

	//	return (mouseButtons[_button] == key_released);
	//}

	/**
	* \brief Function to get the Mouse Position, to a couple of variables.
	* \param x The Reference to Set the X Position to.
	* \param y The reference to set the Y Position to.
	*/
	void GetMousePosition(double& x, double& y) const;

	// TODO: Write Unit Tests for This..
	void Update(const float _deltatime);

private:

	/**
	 * \brief Calls GLFWTerminate
	 */
	static void CleanUp();

};
