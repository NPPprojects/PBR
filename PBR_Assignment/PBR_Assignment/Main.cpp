
#include "stdafx.h"
#include "CustomCursor.h"
#include "Shader.h"
#include "ObjectClass.h"
#include "GameObject.h"
#include "CameraObject.h"
#include "Model.h";
#include "glm/ext.hpp"
#include "FrameBuffer.h"

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
int loadCubemap(std::vector<std::string> _textures);

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
	//Enable stencil Testing
	//Enable Face Culling
	glEnable(GL_CULL_FACE);
	
	//Custom Cursor
	CustomCursor Yugioh_Cursor("resources/textures/YuGiOh Pyramid.png", window);

	//Shader Program
	std::shared_ptr<Shader> TextureShader3D = std::make_shared<Shader>("3DObjectShader.vert", "TextureShader.frag");
	std::shared_ptr<Shader> ColorBoxShader = std::make_shared<Shader>("ColorBox.vert", "ColorBox.frag");
	std::shared_ptr<Shader> LightBoxShader = std::make_shared<Shader>("LightBox.vert", "LightBox.frag");
	std::shared_ptr<Shader> nanosuitShader = std::make_shared<Shader>("MultipleLights.vert", "MultipleLights.frag");
	std::shared_ptr<Shader> nanosuitShader1 = std::make_shared<Shader>("nanosuit.vert", "nanosuit.frag");
	std::shared_ptr<Shader> frameBufferShader = std::make_shared<Shader>("framebufferScreen.vert", "framebufferScreen.frag");
	std::shared_ptr<Shader> cubeMapSkyboxShader = std::make_shared<Shader>("cubemapSkybox.vert", "cubemapSkybox.frag");
	//Testing Shader
	std::shared_ptr<GameObject> LightBox = std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera);
	std::vector<std::shared_ptr<GameObject>> lightBoxes;
	for (int i = 0; i <= 1; i++)
	{
		lightBoxes.push_back(std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera));
	}
	

	std::shared_ptr<GameObject> nanosuit = std::make_shared<GameObject>(nanosuitShader, FPScamera, "resources/objects/nanosuit/nanosuit.obj");
	std::shared_ptr<FrameBuffer> frameBuffer = std::make_shared<FrameBuffer>("framebufferScreen.data", frameBufferShader, FPScamera,ScreenWidth,ScreenHeight);

	//const char *textures[] = { "resources/textures/BetterBox_spec.png" ,"resources/textures/BetterBox_spec.png","resources/textures/Blue_matrix.jpg" };
	//std::vector<std::shared_ptr<GameObject> > EmissionBoxes;
	
	// Dont use raw arrays. use std::array;
	glm::vec3 positions[] = {
		glm::vec3(1.5409f,  0.248259f,  0.822887f),
		glm::vec3(-1.36048f,  0.248259f,  0.822887f),
	};

	
	//Loading a skybox
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	// load textures
	// -------------

	std::vector<std::string> faces
	{
		("resources/textures/skybox/right.jpg"),
		("resources/textures/skybox/left.jpg"),
		("resources/textures/skybox/top.jpg"),
		("resources/textures/skybox/bottom.jpg"),
		("resources/textures/skybox/front.jpg"),
		("resources/textures/skybox/back.jpg")
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	cubeMapSkyboxShader->setInt("skybox", 0);
	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		glm::vec3 tracker = FPScamera->GetPosition();
		std::cout << tracker.x << "  " << tracker.y << "  " << tracker.z << ": WORLD POSITIONS" << std::endl;
		std::cout << "Up/Down: " << FPScamera->getPitch() << "      " << "Left//Right: " << FPScamera->getYaw() << std::endl;
		//std::cout << FPScamera->getPitch() << "Left//Right" << std::endl;
		//Timer
		std::cout << glfwGetTime()<<std::endl;
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

		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
		//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getFBO());

		glClearColor(0.184f, 0.196f, 0.235f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	

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
		
		glm::mat4 view = FPScamera->GetViewMatrix();
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		cubeMapSkyboxShader->use();
		view = glm::mat4(glm::mat3(FPScamera->GetViewMatrix())); // remove translation from the view matrix
	//	glm::mat4 projection = glm::perspective(glm::radians(FPScamera->Getzoom()), (float)800 / (float)600, 0.1f, 100.0f);
		cubeMapSkyboxShader->setMat4("view", view);
//		cubeMapSkyboxShader->setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default


		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
	////	glClearColor(0.184f, 0.196f, 0.235f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	////	glClear(GL_COLOR_BUFFER_BIT);

		// use the color attachment texture as the texture of the quad plane
	////	frameBuffer->use();
		





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


int loadCubemap(std::vector<std::string> _textures)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//Setting up faces of cubes
	int width, height, nrChannel;


	for (int i = 0; i < _textures.size(); i++)   //Iterate through each texture
	{
		unsigned char *data = stbi_load(_textures[i].c_str(), &width, &height, &nrChannel, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << _textures[i] << std::endl;
				stbi_image_free(data);
			}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		
	}
	return textureID;
}
