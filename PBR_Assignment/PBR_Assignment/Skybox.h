#pragma once
#include "ObjectClass.h"

class Skybox :
	public ObjectClass
{
public:

	Skybox(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, std::vector<std::string> &_faces);
	~Skybox();
	int loadCubemap(std::vector<std::string> _textures);
	void use();
	// SET function for view matrix

private:
	std::vector<std::string> faces;
	unsigned int cubemapTexture;

};

