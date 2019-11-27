#include "Skybox.h"



Skybox::Skybox(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _screenHeight) :
	ObjectClass(_ObjectFile, _objectShader, _camera)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	frameBufferConfiguration();
}

Skybox::~Skybox()
{

}
