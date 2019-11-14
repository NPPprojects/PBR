#include "ObjectClass.h"
#include "glm/ext.hpp"
//Create a function that can read a text file containing vertecies for an object and the amount of components per vertex attribute

//HUD
ObjectClass::ObjectClass(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader)
{
	setShader(_objectShader);
	readVertexData(_ObjectFile);
	initialiseVertexData();

}
//No Texture
ObjectClass::ObjectClass(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera)
{
	setShader(_objectShader);
	setCamera(_camera);
	readVertexData(_ObjectFile);
	initialiseVertexData();

}
//Pre-Made Objects
ObjectClass::ObjectClass(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, const char* _ObjectFile )
{
	setShader(_objectShader);
	setCamera(_camera);
	objModel = std::make_shared<Model>(_ObjectFile);
  
}
//Textured
ObjectClass::ObjectClass(const char* _ObjectFile, const char* _TexturePath, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera)
{
	setShader(_objectShader);
	setCamera(_camera);
	readVertexData(_ObjectFile);
	initialiseVertexData();

	texture[0] = loadTexture(_TexturePath);
}


ObjectClass::ObjectClass(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera)
{
	setShader(_objectShader);
	setCamera(_camera);
	readVertexData(_ObjectFile);
	initialiseVertexData();
	for (int i = 0; i <= _TextureCount - 1; i++)
	{

		texture[i] = loadTexture(_TexturePath[i]);
	}

}


ObjectClass::~ObjectClass()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

unsigned int ObjectClass::getVAO()
{
	return VAO;
}

void ObjectClass::use()
{
	objectShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(VAO); // Only one object so no need to bind it every time but good practice
	glDrawArrays(GL_TRIANGLES, 0, verteciesCount);
	//transformations

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void ObjectClass::use(glm::vec3 _position)
{
	objectShader->use();
	applyTransformation(_position);
	unsigned int transformLoc = glGetUniformLocation(objectShader->getID(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);						//Only One Texture
	glBindVertexArray(VAO); // Only one object so no need to bind it every time but good practice
	glDrawArrays(GL_TRIANGLES, 0, verteciesCount);
	//transformations

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void ObjectClass::use3D()
{
	objectShader->use();
	//Textures
	for (int i = 0; i < 4; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);          //Only One Texture
	}
	//Temp Transformations
	temp = temp + 0.01f; //Testing values 
   // std::cout << temp << "This value \n";

	//Set Up Uniforms
	setShaderUniform();

	//Set Up Projection

	projection = glm::perspective(glm::radians(camera->Getzoom()), (float)1920 / (float)1080, 0.1f, 100.0f);
	view = camera->GetViewMatrix();


	//Draw Object

	model = glm::mat4(1.0f);
	glBindVertexArray(VAO); // Only one object so no need to bind it every time but good practice
	glDrawArrays(GL_TRIANGLES, 0, verteciesCount);
	//Disable Shader


	glBindVertexArray(0);
	for (int i = 0; i < sizeof(texture) + 1; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);          //Only One Texture
	}

}

void ObjectClass::useModel()
{
	/*
	model = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera->Getzoom()), (float)1920 / (float)1080, 0.1f, 100.0f);
	view = camera->GetViewMatrix();

	setShaderUniform();
	objectShader->use();
	objModel->Draw(objectShader);	
	*/



	objectShader->use();
	
	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), (float)1920 / (float)1080, 0.1f, 100.0f);
	view = camera->GetViewMatrix();

	

	model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
	
	objModel->Draw(objectShader);
}

void ObjectClass::readVertexData(const char* _ObjectFile)
{
	std::string line;
	std::vector < std::string > Splitline;
	vertexData.open(_ObjectFile);                    //  read-Stream
	getline(vertexData, line);


	splitStringWhitespace(line, Splitline);

	attributeTypeCount = atoi(Splitline.at(0).c_str()); //Assign count of attributes

	verteciesCount = atoi(Splitline.at(1).c_str());
	stride = atoi(Splitline.at(2).c_str());

	for (int i = 0; i < attributeTypeCount; i++)
	{
		vertexPrValue.push_back(i);
		vertexPrValue[i] = atoi(Splitline.at(3 + i).c_str());

	}
	vertexData >> totalVertexFloatCount;            //Total Vertex Float Count


	for (int i = 0; i < totalVertexFloatCount; i++)
	{
		vertexInduvidualData.push_back(i);
		vertexData >> vertexInduvidualData[i];
	//	std::cout << vertexInduvidualData[i] << std::endl;
	}
	vertexData.close();
}

void ObjectClass::splitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
	std::string curr;
	output.clear();
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == ' f ' ||
			input.at(i) == ' , ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}
	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

unsigned int ObjectClass::loadTexture(char const * _TexturePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(_TexturePath, &width, &height, &nrComponents, 0);
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
		std::cout << "Texture failed to load at path: " << _TexturePath << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void ObjectClass::initialiseTransformations()
{
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
}

void ObjectClass::initialiseVertexData()
{
	unsigned int SkipCounter = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexInduvidualData.size() * sizeof(vertexInduvidualData), &vertexInduvidualData.front(), GL_STATIC_DRAW);


	// Set the vertex attributes pointers
	for (int i = 0; i < attributeTypeCount; i++)
	{

		glVertexAttribPointer(i, vertexPrValue[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)((SkipCounter) * sizeof(float)));
	//	std::cout << vertexPrValue[i] << " vertexPrValue" << std::endl;
	//	std::cout << stride << " stride" << std::endl;
	//	std::cout << SkipCounter << " Skip Counter" << std::endl;
		SkipCounter = SkipCounter + vertexPrValue[i];

		glEnableVertexAttribArray(i);
	}
}

void ObjectClass::setShader(std::shared_ptr <Shader> _objectShader)
{
	objectShader = _objectShader;
}

void ObjectClass::setCamera(std::shared_ptr <CameraObject> _camera)
{
	camera = _camera;
}

void ObjectClass::applyTransformation(glm::vec3 _position)
{
	initialiseTransformations();
	transform = glm::translate(transform, _position);

}
void ObjectClass::setDirLightPos(std::shared_ptr<ObjectClass> _lightSrc)
{
	dirLightPos = glm::vec3(_lightSrc->getPosition());
	
}
void ObjectClass::setPointLightPos(glm::vec3 _lightPositions[], int _AmountOfLights)
{
	for (int i = 0; i <= _AmountOfLights; i++)
	{
		pointLightPos[i] = _lightPositions[i];
	}
}
void ObjectClass::setPointLightPos(std::vector<std::shared_ptr<ObjectClass>> _lightPositions, int _AmountOfLights)
{
	
	for (int i = 0; i <= _AmountOfLights; i++)
	{
		pointLightPos[i] = _lightPositions[i]->getPosition();
	}
}
glm::mat4 ObjectClass::getModel()
{
	return model;
}

void ObjectClass::setShaderUniform()
{
	for (int i = 0; i <= 3; i++)


	objectShader->setVec3("LightColor", 1.0f, 1.0f, 1.0f);
	// directional light
	objectShader->setVec3("dirLight.direction", dirLightPos);
	objectShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	objectShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	objectShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);



	std::string number;

	for (int i = 0; i <= 4; i++)
	{
		number = std::to_string(i);
		objectShader->setVec3("pointLights[" + number + "].position", pointLightPos[i]);
		objectShader->setVec3("pointLights[" + number + "].ambient", 0.05f, 0.05f, 0.05f);
		objectShader->setVec3("pointLights[" + number + "].diffuse", 0.8f, 0.8f, 0.8f);
		objectShader->setVec3("pointLights[" + number + "].specular", 1.0f, 1.0f, 1.0f);
		objectShader->setFloat("pointLights[" + number + "].constant", 1.0f);
		objectShader->setFloat("pointLights[" + number + "].linear", 0.09);
		objectShader->setFloat("pointLights[" + number + "].quadratic", 0.032);
	}
	// point light 2
    
  

	objectShader->setMat4("projection", projection);

	objectShader->setMat4("view", view);

	objectShader->setMat4("transform", transform);

	objectShader->setMat4("model", model);

	objectShader->setVec3("light.position", dirLightPos);

	objectShader->setVec3("viewPosition", camera->GetPosition());

	objectShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	objectShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	objectShader->setFloat("material.shininess", 32.0f);
	objectShader->setInt("material.diffuse", 0);
	objectShader->setInt("material.specular", 1);
	objectShader->setInt("material.emission", 2);

	objectShader->setFloat("time", glfwGetTime());
}

glm::vec3 ObjectClass::getPosition()
{
	return model[3];
}


//DEBUG FUNCTIONS
glm::vec3 ObjectClass::getPointLightPos(int index)
{
	return pointLightPos[index];
}

glm::vec3 ObjectClass::getDirLightPos()
{
	return dirLightPos;
}