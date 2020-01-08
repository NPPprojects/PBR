#include "Main.h"

#include "Scene.h"
//Memory Leaks
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>





int main()
{

	glfwInit();   //Initialise GlFW Library
	//specify the client API version that the created context must be compatible with
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Get access to smaller subset of OpenGL features(without backwards compatible features that are no longer needed)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window
	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Fun Times with OpenGl", nullptr, nullptr); //Sets Game to Primary Monitor FullScreen glfwGetPrimaryMonitor()
	//In case of failing to create window terminate glfw
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window \n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Sets the current context on the calling thread since it can only be one at a time

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// Make Window Resisable;
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse and disable the cursor

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Initialise Glew before OpenGL so that it can manage function pointers for OpenGL
	//
	glewInit();

	//Glad Function Pointers initialisation
/*
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD \n";
		return -1;
	}
*/
// Enabling Depth Testing so that fragments that are behind other objects don't get drawn
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//Enable stencil Testing
	//Enable Face Culling
	//glEnable(GL_CULL_FACE);



	//Start Scene From here 
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->initalise(window, FPScamera);
	scene->loadSceneOne();

	system("pause");
	return 0;

}
