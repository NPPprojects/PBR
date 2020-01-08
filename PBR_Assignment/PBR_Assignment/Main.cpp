
#include "stdafx.h"
#include "CustomCursor.h"
#include "Shader.h"
#include "ObjectClass.h"
#include "GameObject.h"
#include "CameraObject.h"
#include "Model.h";
#include "glm/ext.hpp"
#include "FrameBuffer.h"
#include "Skybox.h"

//Memory Leaks
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>

// Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Input Function
void processInput(GLFWwindow *window, std::shared_ptr<FrameBuffer> _framebuffer);


//setings
const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 800;

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

int loadTexture(char const * path);
void renderCube();
float testMovement=1;

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
	// load textures
// -------------



	//Shader Program
	std::shared_ptr<Shader> TextureShader3D = std::make_shared<Shader>("3DObjectShader.vert", "TextureShader.frag");
	std::shared_ptr<Shader> ColorBoxShader = std::make_shared<Shader>("ColorBox.vert", "ColorBox.frag");
	std::shared_ptr<Shader> LightBoxShader = std::make_shared<Shader>("LightBox.vert", "LightBox.frag");
	std::shared_ptr<Shader> nanosuitShader = std::make_shared<Shader>("MultipleLights.vert", "MultipleLights.frag");
	
	std::shared_ptr<Shader> frameBufferShader = std::make_shared<Shader>("framebufferScreen.vert", "framebufferScreen.frag");

	std::shared_ptr<Shader> cubeMapSkyboxShader = std::make_shared<Shader>("cubemapSkybox.vert", "cubemapSkybox.frag");

	//Point Lighting PBR Textured
	std::shared_ptr<Shader> PBRShader = std::make_shared<Shader>("PBR.vert", "PBR.frag");
	
	//IBL Lighting
	std::shared_ptr<Shader> equirectangularShader = std::make_shared<Shader>("EquirectangularCubemap.vert", "EquirectangularCubemap.frag");
	std::shared_ptr<Shader> backgroundShader = std::make_shared<Shader>("background.vert", "background.frag");

	//Objects

	std::shared_ptr<GameObject> LightBox = std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera, ScreenWidth, ScreenHeight);

	std::vector<std::shared_ptr<GameObject>> lightBoxes;

	for (int i = 0; i <= 1; i++)
	{
		lightBoxes.push_back(std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera, ScreenWidth, ScreenHeight));
	}
	
	std::shared_ptr<GameObject> monster = std::make_shared<GameObject>(nanosuitShader, FPScamera, "resources/objects/nadia/SpecialOnes.obj", ScreenWidth, ScreenHeight);
	std::shared_ptr<GameObject> nanosuit = std::make_shared<GameObject>(nanosuitShader, FPScamera, "resources/objects/nanosuit/nanosuit.obj",ScreenWidth, ScreenHeight);
	std::shared_ptr<GameObject> spheres = std::make_shared<GameObject>(PBRShader, FPScamera, ScreenWidth, ScreenHeight);
	
	std::shared_ptr<FrameBuffer> frameBuffer = std::make_shared<FrameBuffer>("framebufferScreen.data", frameBufferShader, FPScamera,ScreenWidth,ScreenHeight);
		
	//IBL Lighting Skybox

	
	// Dont use raw arrays. use std::array;
	glm::vec3 positions[] = {
		glm::vec3(1.5409f,  0.248259f,  0.822887f),
		glm::vec3(-1.36048f,  0.248259f,  0.822887f),
	};

	unsigned int albedo = loadTexture("resources/textures/rusted/rust_basecolor.png");
	unsigned int normal = loadTexture("resources/textures/rusted/rust_normal.png");
	unsigned int metallic = loadTexture("resources/textures/rusted/rust_metallic.png");
	unsigned int roughness = loadTexture("resources/textures/rusted/rust_roughness.png");
	unsigned int ao = loadTexture("resources/textures/rusted/ao.png");


	//Textures for Rusting Iron Ball
	PBRShader->use();
	PBRShader->setInt("albedoMap", 0);
	PBRShader->setInt("normalMap", 1);
	PBRShader->setInt("metallicMap", 2);
	PBRShader->setInt("roughnessMap", 3);
	PBRShader->setInt("aoMap", 4);


	
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
		glm::vec3(150.0f, 150.0f, 150.0f)
	};



	//Framebuffer env
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// pbr: load the HDR environment map
	// ---------------------------------
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(("resources/textures/hdr/Newport_Loft_Ref.hdr"), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	equirectangularShader->use();
	equirectangularShader->setInt("equirectangularMap", 0);
	equirectangularShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(FPScamera->Getzoom()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
	
	backgroundShader->use();
	backgroundShader->setMat4("projection", projection);

	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
	//Render Loop

	while (!glfwWindowShouldClose(window))
	{
		//glm::vec3 tracker = FPScamera->GetPosition();
		//std::cout << tracker.x << "  " << tracker.y << "  " << tracker.z << ": WORLD POSITIONS" << std::endl;
		//std::cout << "Up/Down: " << FPScamera->getPitch() << "      " << "Left//Right: " << FPScamera->getYaw() << std::endl;
		//std::cout << FPScamera->getPitch() << "Left//Right" << std::endl;
		//Timer
		//std::cout << glfwGetTime()<<std::endl;
		double currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		//Input handling
		if (spin == true)
		{
		//	std::cout << "Spinning" << std::endl;
			Time = glfwGetTime();
		}
		else if (spin == false)
		{
		//	std::cout << "Paused" << std::endl;
			glfwSetTime(Time);
			deltaTime = 0.02f;
		}
		processInput(window,frameBuffer);

		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
//		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getFBO());

		glClearColor(0.184f, 0.196f, 0.235f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	
		glm::mat4 view = FPScamera->GetViewMatrix();

		for (int i = 0; i <= 1; i++)
		{
			lightBoxes.at(i)->use3D();
			lightBoxes.at(i)->setPosition(positions[i]);

			lightBoxes.at(i)->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		}
		positions[0] = glm::vec3(7*sin(Time), testMovement, 7*cos(Time));
		positions[1] = glm::vec3(-7*sin(Time), 1.92578f, -7*cos(Time));

		
 
		LightBox->use3D();
		LightBox->setPosition(glm::vec3(1.8f*sin(Time) , 0.90f, 1.0f*cos(Time)));
		LightBox->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

		
		nanosuit->useModel();
		nanosuit->setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
		nanosuit->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		nanosuit->setDirLightPos(LightBox);  
		nanosuit->setPointLightPos(lightBoxes, 1);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);

		spheres->useSphere();


		for (unsigned int i = 0; i <= 1; ++i)
		{	
			PBRShader->setVec3("lightPositions[" + std::to_string(i) + "]", lightBoxes[i]->getPosition());
			PBRShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
		}

		//skybox->use();

		backgroundShader->use();
		backgroundShader->setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		renderCube();


		////now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		////clear all relevant buffers
		//glClearColor(0.184f, 0.196f, 0.235f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		////use the color attachment texture as the texture of the quad plane

		//frameBuffer->use();

	
	



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

void processInput(GLFWwindow *window, std::shared_ptr<FrameBuffer> _framebuffer)
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		_framebuffer->setGamma(0.01);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		_framebuffer->setGamma(-0.01);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		_framebuffer->setExposure(0.01);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		_framebuffer->setExposure(-0.01);

		//Test Movement
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		testMovement = testMovement + 0.1;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		testMovement = testMovement - 0.1;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
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

	FPScamera->ProcessMouseMovement(xoffset, yoffset,true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FPScamera->ProcessMouseScroll(yoffset);
}

// renders (and builds at first invocation) a sphere
// -------------------------------------------------



// utility function for loading a 2D texture from file
// ---------------------------------------------------
int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}