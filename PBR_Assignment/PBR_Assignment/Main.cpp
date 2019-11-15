
#include "stdafx.h"
#include "CustomCursor.h"
#include "Shader.h"
#include "ObjectClass.h"
#include "GameObject.h"
#include "CameraObject.h"
#include "Model.h";
#include "glm/ext.hpp"


//Memory Leaks
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>

// Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Input Function
void processInput(GLFWwindow *window);


//setings
const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 600;

//Testing value

bool spin = true;

double Time;
//FPS Camera
std::shared_ptr<CameraObject> FPScamera = std::make_shared<CameraObject>();
double deltaTime;
double lastFrame;
double lastX = ScreenWidth / 2.0f;
double lastY = ScreenHeight / 2.0f;

bool firstMouse = true;

float Temp_Test = 0.0f; //Value for testing

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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


	//Custom Cursor
	CustomCursor Yugioh_Cursor("resources/textures/YuGiOh Pyramid.png", window);

	//Shader Program
	std::shared_ptr<Shader> TextureShader3D = std::make_shared<Shader>("3DObjectShader.vert", "TextureShader.frag");
	std::shared_ptr<Shader> ColorBoxShader = std::make_shared<Shader>("ColorBox.vert", "ColorBox.frag");
	std::shared_ptr<Shader> LightBoxShader = std::make_shared<Shader>("LightBox.vert", "LightBox.frag");
	std::shared_ptr<Shader> nanosuitShader = std::make_shared<Shader>("MultipleLights.vert", "MultipleLights.frag");
	std::shared_ptr<Shader> nanosuitShader1 = std::make_shared<Shader>("nanosuit.vert", "nanosuit.frag");
	//Testing Shader
	std::shared_ptr<GameObject> LightBox = std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera);
	std::vector<std::shared_ptr<GameObject>> lightBoxes;
	for (int i = 0; i <= 1; i++)
	{
		lightBoxes.push_back(std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera));
	}

	std::shared_ptr<GameObject> nanosuit = std::make_shared<GameObject>(nanosuitShader, FPScamera, "resources/objects/nanosuit/nanosuit.obj");

	//const char *textures[] = { "resources/textures/BetterBox_spec.png" ,"resources/textures/BetterBox_spec.png","resources/textures/Blue_matrix.jpg" };
	//std::vector<std::shared_ptr<GameObject> > EmissionBoxes;
	


	// Dont use raw arrays. use std::array;
	glm::vec3 positions[] = {
		glm::vec3(1.5409f,  0.248259f,  0.822887f),
		glm::vec3(-1.36048f,  0.248259f,  0.822887f),
	};

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		glm::vec3 tracker = FPScamera->GetPosition();
		std::cout << tracker.x << "  " << tracker.y << "  " << tracker.z << ": WORLD POSITIONS" << std::endl;
		std::cout << "Up/Down: " << FPScamera->getPitch() << "      " << "Left//Right: " << FPScamera->getYaw() << std::endl;
		//std::cout << FPScamera->getPitch() << "Left//Right" << std::endl;
		//Timer

		double currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//Input handling
		if (spin == true)
		{
			std::cout << "Spinning" << std::endl;
			Time = glfwGetTime();
		}
		else if (spin == false)
		{
			std::cout << "Paused" << std::endl;
			glfwSetTime(Time);
			deltaTime = 0.02f;
		}
		processInput(window);

		//Render
		glClearColor(0.184f, 0.196f, 0.235f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!



		for (int i = 0; i <= 1; i++)
		{
			lightBoxes.at(i)->use3D();
			lightBoxes.at(i)->setPosition(positions[i]);
			//lightBoxes[i]->setRotation(45.0f * Time, positions[i]);
			lightBoxes.at(i)->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		}
		positions[0] = glm::vec3(1.5409f*sin(Time), 0.248259f, 0.822887f*cos(Time));
		positions[1] = glm::vec3(-1.5409f*sin(Time), 1.92578f, -0.822887f*cos(Time));


 
		LightBox->use3D();
		LightBox->setPosition(glm::vec3(1.8f*sin(Time) , 0.90f, 1.0f*cos(Time)));
		LightBox->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

		
		nanosuit->useModel();
		nanosuit->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		nanosuit->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		nanosuit->setDirLightPos(LightBox);  
		nanosuit->setPointLightPos(lightBoxes, 1);
		for (int i = 0; i <= 1; i++)
		{
	//		std::cout << glm::to_string(nanosuit->getPointLightPos(i)) << std::endl;
      
		}
		
		//swap buffers and poll IO events key pressed/released, mosuse moved etc.
		glfwSwapBuffers(window);
		glfwPollEvents();

		//glfw: terminate, clearning all previously allocated GLFW resources

	}



	//EmissionBoxes.clear();
	//lightBoxes.clear();
	glfwTerminate();
	//delete nanosuitTest;*/

	system("pause");
	return 0;

}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		spin = false;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		spin = true;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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


