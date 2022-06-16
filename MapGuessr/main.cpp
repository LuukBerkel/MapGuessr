#include <iostream>
#include <curl/curl.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "objectComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <rapidxml/rapidxml.hpp>
#include "fpsCamera.h"

#include "objectComponent.h"

using tigl::Vertex;

GLFWwindow* window;
fpsCamera* camera;
gameObject* object;

void init()
{
	tigl::init();
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
	camera = new fpsCamera(window);

	object = new gameObject();
	object->scale = glm::vec3(0.001f);
	object->position = glm::vec3(0.0f);
	object->addComponent(std::make_shared<objectComponent>("resources/evergiven/EverGiven.obj"));
}

void update()
{
	
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);

	tigl::begin(GL_TRIANGLES);
	tigl::addVertex(Vertex::PC(glm::vec3(-2, -1, -4), glm::vec4(1, 0, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(2, -1, -4), glm::vec4(0, 1, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(0, 1, -4), glm::vec4(0, 0, 1, 1)));


	tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, 10), glm::vec4(1, 1, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));

	tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(10, -1, -10), glm::vec4(1, 1, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));

	tigl::end();

	object->draw();


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
		camera->update(window);
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
