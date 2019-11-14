#pragma once
#include "stdafx.h"
class CustomCursor
{
private:
	int width, height, nrChannels;
//	std::unique_ptr<unsigned char> *pixels;
	unsigned char *pixels;
	std::unique_ptr<GLFWimage> image;
	GLFWcursor* cursor;
public:
	CustomCursor(const char*, GLFWwindow*);
	~CustomCursor();

	void LoadTexture(const char*);
	void ChangeCursor(GLFWwindow*);
};

