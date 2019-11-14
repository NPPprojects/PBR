#pragma once
#include "ObjectClass.h"

class GameObject : public ObjectClass
{
public:

	GameObject(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera);

	GameObject(const char* _ObjectFile, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera);

	GameObject(std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, const char * _ObjectFile);


	~GameObject();



	void setRotation(float _degree, glm::vec3 _rotation);

	void setPosition(glm::vec3);

	void setScale(glm::vec3 _scale);

  glm::vec3 getPosition();

  glm::mat4 getModelMatrix();

private:


};
