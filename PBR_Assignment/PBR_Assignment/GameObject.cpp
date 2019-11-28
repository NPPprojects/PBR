#include "GameObject.h"



GameObject::GameObject(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _ScreenWidth, int _ScreenHeight) :
	ObjectClass(_ObjectFile, _TexturePath, _TextureCount, _objectShader, _camera , _ScreenWidth, _ScreenHeight)
{

}
GameObject::GameObject(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _ScreenWidth, int _ScreenHeight) :
	ObjectClass(_ObjectFile, _objectShader, _camera, _ScreenWidth, _ScreenHeight)
{

}
GameObject::GameObject(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, const char* _ObjectFile , int _ScreenWidth, int _ScreenHeight) :
	ObjectClass(_objectShader, _camera,  _ObjectFile , _ScreenWidth,  _ScreenHeight)
{

}


GameObject::~GameObject()
{
	std::cout << "removed game object" << std::endl;
}

void GameObject::setRotation(float _degree, glm::vec3 _rotation)
{
	model = glm::rotate(model, _degree, _rotation);
}
void GameObject::setPosition(glm::vec3 _position)
{
	model = glm::translate(model, _position);
}
void GameObject::setScale(glm::vec3 _scale)
{
	model = glm::scale(model, _scale);
}
glm::vec3 GameObject::getPosition()
{
  return model[3];
}
glm::mat4 GameObject::getModelMatrix()
{
  return model;
}

void GameObject::setDirLightPos(std::shared_ptr<GameObject> _lightSrc)
{
	glm::vec3 _Temp = glm::vec3(_lightSrc->getPosition());
	dirLightPos = _Temp;
}

void GameObject::setPointLightPos(std::vector<std::shared_ptr<GameObject>> _lightPositions, int _AmountOfLights)
{

	for (int i = 0; i <= _AmountOfLights; i++)
	{
		pointLightPos[i] = _lightPositions[i]->getPosition();
	}
}

