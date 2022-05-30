#include <GL/glew.h>
#include "renderController.h"
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "objectComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using tigl::Vertex;

objectComponent component;

void renderController::init()
{
	tigl::init();
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
	component =  objectComponent("resources/car/honda_jazz.obj");
}

void renderController::update()
{
}

void renderController::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), 1920.0f / 1080,0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCN(glm::vec3(10, 100, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(10, 100, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(100, 100, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(100, 100, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::end();


	component.draw();
}


// Boots up the graphics and window
renderController::renderController()
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
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Closing application
	glfwTerminate();
}
