#include "ObjectClass.h"
#include "glm/ext.hpp"
//Create a function that can read a text file containing vertecies for an object and the amount of components per vertex attribute

//HUD
ObjectClass::ObjectClass(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, int _screenWidth, int _screenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	readVertexData(_ObjectFile);
	initialiseVertexData();

}
//No Texture
ObjectClass::ObjectClass(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	setCamera(_camera);
	readVertexData(_ObjectFile);
	initialiseVertexData();
}
//Sphere
ObjectClass::ObjectClass(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	setCamera(_camera);

	


	initialiseVertexSphereData();
}
//Pre-Made Objects
ObjectClass::ObjectClass(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, const char* _ObjectFile,int _screenWidth,int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	setCamera(_camera);
	objModel = std::make_shared<Model>(_ObjectFile);
  
}
//Textured
ObjectClass::ObjectClass(const char* _ObjectFile, const char* _TexturePath, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	setCamera(_camera);
	readVertexData(_ObjectFile);
	initialiseVertexData();
	
	texture[0] = loadTexture(_TexturePath);
}


ObjectClass::ObjectClass(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
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
	
   // std::cout << temp << "This value \n";

	//Set Up Uniforms
	setShaderUniform();

	//Set Up Projection

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
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
	objectShader->use();
	
	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
	view = camera->GetViewMatrix();

	model = glm::mat4(1.0f);	
	objModel->Draw(objectShader);
}
void ObjectClass::useTextureSphere()
{

	objectShader->use();

//	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
	view = camera->GetViewMatrix();
	objectShader->setMat4("projection", projection);
	objectShader->setMat4("view", view);
	objectShader->setVec3("camPos", camera->GetPosition());

	//1 Ball
	//model = glm::mat4(1.0f);
	//objectShader->setMat4("model", model);

	//Multiple
	int nrRows = 1000;
	int nrColumns = 10;
	float spacing = 2.5;

	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrColumns; ++col)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));
			objectShader->setMat4("model", model);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}
	}


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

}
void ObjectClass::useSphere()
{

	objectShader->use();

	//	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
	view = camera->GetViewMatrix();
	objectShader->setMat4("projection", projection);
	objectShader->setMat4("view", view);
	objectShader->setVec3("camPos", camera->GetPosition());

	//1 Ball
	//model = glm::mat4(1.0f);
	//objectShader->setMat4("model", model);

	//Multiple
	int nrRows = 50;
	int nrColumns = 50;
	float spacing = 2.5;

	for (int row = 0; row < nrRows; ++row)
	{
		objectShader->setFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			objectShader->setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (nrColumns / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f
			));
			objectShader->setMat4("model", model);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
	}
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
			input.at(i) == 'f' ||
			input.at(i) == ',' ||
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
	glBufferData(GL_ARRAY_BUFFER, vertexInduvidualData.size() * sizeof(vertexInduvidualData.at(0)), &vertexInduvidualData.at(0), GL_STATIC_DRAW);


	// Set the vertex attributes pointers
	for (int i = 0; i < attributeTypeCount; i++)
	{

		glVertexAttribPointer(i, vertexPrValue[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)((SkipCounter) * sizeof(float)));
		SkipCounter = SkipCounter + vertexPrValue[i];
		glEnableVertexAttribArray(i);
	}
}
void ObjectClass::initialiseVertexSphereData()
{
	
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}
	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

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


glm::mat4 ObjectClass::getModel()
{
	return model;
}

void ObjectClass::setShaderUniform()
{
	objectShader->setVec3("LightColor", 1.0f, 1.0f, 1.0f);
	// directional light
	objectShader->setVec3("dirLight.direction", dirLightPos);
	objectShader->setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	objectShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	objectShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);



	std::string number;
	//Point Lights
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
	
  
	//Projection matrix
	objectShader->setMat4("projection", projection);
	//View Matrix
	objectShader->setMat4("view", view);
	//Transformation Matrix
	objectShader->setMat4("transform", transform);
	//Model Matrix
	objectShader->setMat4("model", model);
	//directional light position
	objectShader->setVec3("light.position", dirLightPos);
	//directional light Values
	objectShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	objectShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
	objectShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//camera position
	objectShader->setVec3("viewPosition", camera->GetPosition());
	//material values
	objectShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	objectShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	objectShader->setFloat("material.shininess", 32.0f);
	objectShader->setInt("material.diffuse", 0);
	objectShader->setInt("material.specular", 1);
	objectShader->setInt("material.emission", 2);
	//time value
	objectShader->setFloat("time", glfwGetTime());

	//FrameBuffer
	objectShader->setInt("screenTexture", 0);

	//Skybox 
	objectShader->setInt("skybox", 0);

	//PBR Not Optimised 
	objectShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
	objectShader->setFloat("ao", 1.0f);
	objectShader->setVec3("CamPos", camera->GetPosition());

	objectShader->setInt("albedoMap", 0);
	objectShader->setInt("normalMap", 1);
	objectShader->setInt("metallicMap", 2);
	objectShader->setInt("roughnessMap", 3);
	objectShader->setInt("aoMap", 4);


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

void ObjectClass::setScreenParameters(int _screenWidth, int _screenHeight)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
}