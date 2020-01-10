#include "SphereClass.h"

#include "stdafx.h"
#include "Shader.h"
#include "CameraObject.h"


SphereClass::SphereClass(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _ScreenHeight)
{
	setScreenParameters(_screenWidth, _screenWidth);
	setShader(_objectShader);
	setCamera(_camera);
}


SphereClass::~SphereClass()
{
}

void SphereClass::initialiseSphere()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::vector<glm::vec3> positions;

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
	float stride = (3 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

}

void SphereClass::initaliseTextureSphere()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

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

void SphereClass::setScreenParameters(int _screenWidth, int _screenHeight)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
}

void SphereClass::setPosition(glm::vec3 _position)
{
	position = _position;
}

void SphereClass::setScale(glm::vec3 _scale)
{
	scale = _scale;
}

void SphereClass::updateModelMatrix()
{
	model = glm::translate(model,position);
	model = glm::scale(model,scale);
}

void SphereClass::setShader(std::shared_ptr <Shader> _objectShader)
{
	objectShader = _objectShader;
}

void SphereClass::setCamera(std::shared_ptr <CameraObject> _camera)
{
	camera = _camera;
}

void SphereClass::useSphere()
{

	objectShader->use();

	//	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
	view = camera->GetViewMatrix();
	objectShader->setMat4("projection", projection);
	objectShader->setMat4("view", view);
	objectShader->setVec3("camPos", camera->GetPosition());

	//1 Ball
	model = glm::mat4(1.0f);
	

	//Multiple
	nrRows = 10;
	nrColumns = 10;
	spacing = 2.5f;

	


	for (int row = 0; row < nrRows; ++row)
	{
		objectShader->setFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
		
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
	}
		//objectShader->setFloat("metallic", 0.5f);
		//objectShader->setFloat("roughness", 0.3f);
		//objectShader->setMat4("model", model);
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
	}


void SphereClass::useTextureSphere()
{


	objectShader->use();

	//	setShaderUniform();

	projection = glm::perspective(glm::radians(camera->Getzoom()), float(screenWidth) / (float)screenHeight, 0.1f, 100.0f);
	view = camera->GetViewMatrix();
	objectShader->setMat4("projection", projection);
	objectShader->setMat4("view", view);
	objectShader->setVec3("camPos", camera->GetPosition());

	//1 Ball
	model = glm::mat4(1.0f);


	//Multiple
	nrRows = 10;
	nrColumns = 10;
	spacing = 2.5f;

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

	//objectShader->setMat4("model", model);
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

}

