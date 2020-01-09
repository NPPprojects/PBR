#pragma once
#include "stdafx.h"

#include "Shader.h"

#include <fstream>

#include <iostream>

#include "CameraObject.h"

#include "Model.h"


class ObjectClass
{
public:
	//HUD
	ObjectClass(const char* _ObjectFile, std::shared_ptr<Shader> _objectShader, int _ScreenWidth, int _ScreenHeight);
	//No Texture
	ObjectClass(const char*, std::shared_ptr <Shader>, std::shared_ptr <CameraObject> _camera, int _ScreenWidth, int _ScreenHeight);

	ObjectClass(std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, int _screenWidth, int _ScreenHeight);
	//Pre-Made
	ObjectClass(std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, const char * _ObjectFile, int _ScreenWidth, int _ScreenHeight);
	//Textured
	ObjectClass(const char * _ObjectFile, const char * _TexturePath, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, int _ScreenWidth, int _ScreenHeight);

	//Experimental
	ObjectClass(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, int _ScreenWidth, int _ScreenHeight);

	~ObjectClass();

	unsigned int getVAO();

	void use();

	void use(glm::vec3);

	void use3D();

	void useModel();

	void useTextureSphere();

	void useSphere();
	

	glm::mat4 getModel();

	void setShaderUniform();

	glm::vec3 getPosition();


	unsigned int loadTexture(char const *);

	void initialiseTransformations();

	void setShader(std::shared_ptr <Shader> _objectShader);

	void setCamera(std::shared_ptr<CameraObject> _camera);

	void applyTransformation(glm::vec3);

	void setScreenParameters(int _screenWidth, int _screenHeight);
	






	//Debug Functions
	glm::vec3 getPointLightPos(int index);

	glm::vec3 getDirLightPos();

	





private:
	//Vertex Data Initialisation
	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object, Element Buffer Object
	unsigned int indexCount; //For EBO
	int attributeTypeCount; //Counter for amount of Attributes
	int stride;     //The stride
	std::ifstream vertexData; //VertexDataFile
	int totalVertexFloatCount;       //Total Vertex Amount
	std::vector<float> vertexInduvidualData; //All vertex data
	std::vector<int> vertexPrValue; //Layout number


	void splitStringWhitespace(std::string&, std::vector<std::string>&);
	void readVertexData(const char*);
	void initialiseVertexData();         //set up vertex data (and buffer(s)) and configure vertex attributes
	void initialiseVertexSphereData();		//set up vertex data (and buffer(s)) and configure vertex attributes for sphere 
protected:

	int verteciesCount; //Amount of vertecies in an object

	//Textures
	unsigned int texture[8]; //Texture Of Object



	// TODO:
	// Never store matrix
	// vec3 pos, rot, scale
	// return glm::translate, glm::rotate x 3 (rot), glm::scale (scale)
  //Transformations
	glm::mat4 transform;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	//Shader
	std::shared_ptr <Shader> objectShader;

	//Camera
	std::shared_ptr <CameraObject> camera;

	//Used to get location of light source
	glm::vec3 dirLightPos;

	//Used to get location of point light sources
	glm::vec3 pointLightPos[4];

	//Model
	std::shared_ptr<Model> objModel;

	//Screen Resolution
	int screenWidth;
	int screenHeight;


	//Values I need to delete
		// lights
	// ------


};


