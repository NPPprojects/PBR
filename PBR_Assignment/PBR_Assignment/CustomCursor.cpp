#include "CustomCursor.h"



CustomCursor::CustomCursor(const char* _path, GLFWwindow* _window)
{
	LoadTexture(_path);
	ChangeCursor(_window);
}


CustomCursor::~CustomCursor()
{
	
}

void CustomCursor::LoadTexture(const char* _path)
{
	pixels = stbi_load(_path, &width, &height, &nrChannels, 0);
	memset(pixels, 255, sizeof(pixels));
	image = std::make_unique <GLFWimage>();
	image->width = width;
	image->height = height;
	image->pixels = pixels;
}
void CustomCursor::ChangeCursor(GLFWwindow* _window)
{
	cursor = glfwCreateCursor(image.get(), 0, 0);
	glfwSetCursor(_window, cursor);
}
