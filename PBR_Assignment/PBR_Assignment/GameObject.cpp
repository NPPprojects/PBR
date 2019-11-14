#include "GameObject.h"



GameObject::GameObject(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera) :
	ObjectClass(_ObjectFile, _TexturePath, _TextureCount, _objectShader, _camera)
{

}
GameObject::GameObject(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera) :
	ObjectClass(_ObjectFile, _objectShader, _camera)
{

}
GameObject::GameObject(std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, const char* _ObjectFile) :
	ObjectClass(_objectShader, _camera,  _ObjectFile)
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



///TODo

//Step 1 
//3 Vectors
//Only values the game Objects stores 
//Constructs them off the class