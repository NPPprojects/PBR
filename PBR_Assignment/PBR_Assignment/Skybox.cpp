#include "Skybox.h"

//Remove when done debugging
#include "glm/ext.hpp"

Skybox::Skybox(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, std::vector<std::string> &_faces) :
	ObjectClass(_ObjectFile, _objectShader, _camera)
{
	
	for (int i = 0; i < 6; i++)
	{
		faces.push_back(_faces[i]);
	}
	cubemapTexture = loadCubemap(faces);
//	faces.clear();
//	_faces.clear();
}

Skybox::~Skybox()
{

}

int Skybox::loadCubemap(std::vector<std::string> _textures)
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

void Skybox::use()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	objectShader->use();
	view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	projection = glm::perspective(glm::radians(camera->Getzoom()), (float)800 / (float)600, 0.1f, 100.0f);
	//Projection matrix
	setShaderUniform();
	//View Matrix


	//objectShader->setMat4("view", view);
	// skybox cube
	glBindVertexArray(getVAO());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);	//Will always be 36 as skybox is just a cube
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}
