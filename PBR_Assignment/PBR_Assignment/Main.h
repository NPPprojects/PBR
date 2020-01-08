#pragma once
#include "CameraObject.h"
bool firstMouse = true;
GLFWwindow* window;
const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 800;
float lastX = ScreenWidth / 2.0f;
float lastY = ScreenHeight / 2.0f;


std::shared_ptr<CameraObject> FPScamera = std::make_shared<CameraObject>();


//Functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	FPScamera->ProcessMouseMovement(xoffset, yoffset, 1);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FPScamera->ProcessMouseScroll(yoffset);
}


