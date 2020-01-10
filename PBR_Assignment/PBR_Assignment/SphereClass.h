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

	void setPosition(glm::vec3 _position);

	void setScale(glm::vec3 _scale);

	void updateModelMatrix();

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

	glm::vec3 position;
	glm::vec3 scale;
	//Shader
	std::shared_ptr <Shader> objectShader;

	//Camera
	std::shared_ptr <CameraObject> camera;

	int screenWidth;
	int screenHeight;

	//Collumn Rows and spacing for multiple spheres
	int nrRows;
	int nrColumns;
	float spacing;
};

