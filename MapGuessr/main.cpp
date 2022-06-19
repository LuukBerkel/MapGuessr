#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "objectComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "fpsCamera.h"
#include "floorComponent.h"

#include "objectComponent.h"
#include "tileCollector.h"
#include "gameScene.h"

using tigl::Vertex;

GLFWwindow* window;
std::shared_ptr<fpsCamera> camera;
std::shared_ptr<gameScene> scene;
float lastFrameTime;

void init()
{
	tigl::init();
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
	camera = std::make_shared<fpsCamera>(window);
	scene = std::make_shared<gameScene>(camera);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, (glm::vec3(0, 7, 12)));
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(1, 1, 1));
	tigl::shader->setShinyness(0);


}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Updating scene.
	scene->update(deltaTime);
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

	scene->draw();

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
