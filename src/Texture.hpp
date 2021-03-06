#pragma once

#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GraphX.hpp"

class Texture
{
private:
	SDL_Surface* res;
	void Load();
	int pixel_height, pixel_width;
public:
	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}
	GLuint texture_id;

	int GetPixelHeight()
	{
		return pixel_height;
	}
	int GetPixelWidth()
	{
		return pixel_width;
	}

	Texture(std::string path);
	Texture(SDL_Surface* s);
	~Texture();
};

