#pragma once
#include "ObjectClass.h"
class Skybox :
	public ObjectClass
{
public:

	Skybox(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _screenHeight);
	~Skybox();


private:

};

