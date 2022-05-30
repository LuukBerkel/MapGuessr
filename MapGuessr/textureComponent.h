#pragma once

#include <gl/glew.h>
#include <string>

class textureComponent
{
	GLuint id;
public:
	textureComponent(const std::string& fileName);
	void bind();
};
