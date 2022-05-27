#pragma once

#include <thread>
#include <GLFW/glfw3.h>

/**
 * @brief Controller class used for booting up and managing graphics thread.
*/
class renderController {
private:
	// Pointer for the window
	GLFWwindow* window;

	// Draw and update functions graphics
	void init();
	void update();
	void draw();
public:
	/**
	 * @brief Boots up the graphics  in a infinite loop. Warning is blocking call!
	*/
	renderController();
};
