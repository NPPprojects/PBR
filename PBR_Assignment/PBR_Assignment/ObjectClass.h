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
	ObjectClass(const char* _ObjectFile, std::shared_ptr<Shader> _objectShader);
	//No Texture
	ObjectClass(const char*, std::shared_ptr <Shader>, std::shared_ptr <CameraObject> _camera);
	//Pre-Made
	ObjectClass(std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, const char * _ObjectFile);
	//Textured
	ObjectClass(const char * _ObjectFile, const char * _TexturePath, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera);

	//Experimental
	ObjectClass(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera);

	~ObjectClass();

	unsigned int getVAO();

	void use();

	void use(glm::vec3);

	void use3D();

	void useModel();

	void setDirLightPos(std::shared_ptr<ObjectClass> _lightSrc);

	void setPointLightPos(glm::vec3 _lightPositions[], int _AmountOfLights);

	void setPointLightPos(std::vector<std::shared_ptr<ObjectClass>> _lightPositions, int _AmountOfLights);

	

	glm::mat4 getModel();

	void setShaderUniform();

	glm::vec3 getPosition();


	unsigned int loadTexture(char const *);

	void initialiseTransformations();

	void setShader(std::shared_ptr <Shader> _objectShader);

	void setCamera(std::shared_ptr<CameraObject> _camera);

	void applyTransformation(glm::vec3);

	float temp = 0.0f; // delete this





	//Debug Functions


	glm::vec3 getPointLightPos(int index);

	glm::vec3 getDirLightPos();


private:
	//Vertex Data Initialisation
	unsigned int VBO, VAO; //Vertex Buffer Object, Vertex Array Object
	int attributeTypeCount; //Counter for amount of Attributes
	int verteciesCount; //Amount of vertecies in an object
	int stride;     //The stride
	std::ifstream vertexData; //VertexDataFile
	int totalVertexFloatCount;       //Total Vertex Amount
	std::vector<float> vertexInduvidualData; //All vertex data
	std::vector<int> vertexPrValue; //Layout number


	void splitStringWhitespace(std::string&, std::vector<std::string>&);
	void readVertexData(const char*);
	void initialiseVertexData();         //set up vertex data (and buffer(s)) and configure vertex attributes
protected:
	//Textures
	unsigned int texture[8]; //Texture Of Object



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
};