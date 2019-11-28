#pragma once
#include "ObjectClass.h"

class Skybox :
	public ObjectClass
{
public:

	Skybox(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _screenHeight, std::vector<std::string> &_faces);
	~Skybox();
	int loadCubemap(std::vector<std::string> _textures);
	void use();



private:
	std::vector<std::string> faces;
	unsigned int cubemapTexture;

};

