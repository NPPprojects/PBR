#pragma once
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
#include "SphereClass.h"
class Scene
{
public:

	//Initialise Scene
	//Render Loop 

	void initalise(GLFWwindow* Window, std::shared_ptr<CameraObject> _camera);
	void loadSceneOne();
	Scene();
	~Scene();
	

	void processInput(GLFWwindow *window, std::shared_ptr<FrameBuffer> _framebuffer);
	int loadTexture(char const * path);
	void renderCube();

private:
	//Time Values
	double deltaTime;
	double lastFrame;
	//GameTime
	double Time;
	//GameTime Control
	bool spin;
	//Vaos for SkyBox
	unsigned int cubeVAO;
	unsigned int cubeVBO;

	//Screen Values
	const unsigned int ScreenWidth = 800;
	const unsigned int ScreenHeight = 800;
	GLFWwindow* window;

	//Camera Pointer
	std::shared_ptr<CameraObject> FPScamera;
	//Camera Default values
	float lastX = ScreenWidth / 2.0f;
	float lastY = ScreenHeight / 2.0f;

	//Shader Program
	std::shared_ptr<Shader> TextureShader3D;
	std::shared_ptr<Shader> ColorBoxShader;
	std::shared_ptr<Shader> LightBoxShader;
	std::shared_ptr<Shader> nanosuitShader;

	std::shared_ptr<Shader> frameBufferShader;

	std::shared_ptr<Shader> cubeMapSkyboxShader;

	//Blinn-Phonng Shader
	std::shared_ptr<Shader> BPShader;
	bool blinn; //For switching between Blinn-Phong shading and Phong Shading
	bool blinnKeyPressed;
	//Point Light PBR
	std::shared_ptr<Shader> PBRShader;
	//Point Light PBR Textured
	std::shared_ptr<Shader> PBRShaderTextured;
	
	//IBL Lighting
	std::shared_ptr<Shader> equirectangularShader;
	std::shared_ptr<Shader> backgroundShader;

	//Objects

	std::shared_ptr<GameObject> LightBox;

	std::vector<std::shared_ptr<GameObject>> lightBoxes;


	std::shared_ptr<GameObject> monster;
	std::shared_ptr<GameObject> nanosuit;
	std::shared_ptr<ObjectClass> equirectangularCube;
	std::shared_ptr<FrameBuffer> frameBuffer;


	std::shared_ptr<Skybox> skybox;

	//Spheres PBR
	std::shared_ptr<SphereClass> spheresPBR;
	std::shared_ptr<SphereClass> texturedSpheresPBR;

	//Spheres Blinn-Phongg
	std::shared_ptr<SphereClass> spheresBP;
	//IBL Lighting Skybox


	//const char *textures[] = { "resources/textures/BetterBox_spec.png" ,"resources/textures/BetterBox_spec.png","resources/textures/Blue_matrix.jpg" };
	//std::vector<std::shared_ptr<GameObject> > EmissionBoxes;

	// Dont use raw arrays. use std::array;
	std::vector<glm::vec3> positions;

	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;

	//Light Brightness values
	std::vector<glm::vec3> lightColors;

	//FrameBuffer VAO and VBO;
	unsigned int captureFBO;
	unsigned int captureRBO;
	//ENV cubemap
	unsigned int envCubemap;


};

