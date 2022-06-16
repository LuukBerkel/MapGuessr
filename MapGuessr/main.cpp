#include <iostream>
#include <curl/curl.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "objectComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <rapidxml/rapidxml.hpp>

using tigl::Vertex;

GLFWwindow* window;

void init()
{
	tigl::init();
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

void update()
{
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), 1920.0f / 1080, 0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);
}


// Boots up the graphics and window
int main(void)
{
	// Inits glfw window manager
	if (!glfwInit())
		throw "Could not initialize glwf";

	// Boots a fullscreen window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	window = glfwCreateWindow(mode->width, mode->height, "MapGuessr", glfwGetPrimaryMonitor(), NULL);

	// Checks if the window is created
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	// Inits the other components
	init();

	// Rendering time...
	while (!glfwWindowShouldClose(window))
	{
		// Checks if there are any gl calls available.
		loadObjectIterate();

		// Updates the motion and physics.
		update();

		// Drawing en showing on window
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Closing application
	glfwTerminate();

	return 0;
}
