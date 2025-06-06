#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <GL/glew.h>
#include "stb_image.h"

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType);
	Texture(const unsigned char* buffer, int len, const char* texType, GLuint slot);
	Texture()
	{
		ID = 0;
		type = nullptr;
		unit = 0;
	}

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif