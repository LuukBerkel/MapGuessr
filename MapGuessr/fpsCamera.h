#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class fpsCamera
{
public:
	fpsCamera(GLFWwindow*);

	glm::mat4 getMatrix();
	void update(GLFWwindow*);

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec2 rotation = glm::vec2(0, 0);

private:


	void move(float angle, float fac);
};
