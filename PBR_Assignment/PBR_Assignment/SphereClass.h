#pragma once
#include "ObjectClass.h"

#include "stdafx.h"
#include "Shader.h"
#include "CameraObject.h"

class SphereClass
{
public:
	SphereClass(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight);
	~SphereClass();
	void initialiseSphere();

	void initaliseTextureSphere();

	void setScreenParameters(int _screenWidth, int _screenHeight);

	void setShader(std::shared_ptr<Shader> _objectShader);

	void setCamera(std::shared_ptr<CameraObject> _camera);

	void useSphere();

	void useTextureSphere();

private:

	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object, Element Buffer Object
	
	unsigned int indexCount;
	glm::mat4 transform;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	//Shader
	std::shared_ptr <Shader> objectShader;

	//Camera
	std::shared_ptr <CameraObject> camera;

	int screenWidth;
	int screenHeight;

	//Collumn Rows and spacing for multiple spheres
	int nrRows = 10;
	int nrColumns = 10;
	float spacing = 2.5;
};

