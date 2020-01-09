#pragma once
#include "ObjectClass.h"

class GameObject : public ObjectClass
{
public:

	//Basic objects with position with/without colors
	GameObject(const char* _ObjectFile, std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, int _ScreenWidth, int _ScreenHeight);
	
	//Basic objects with textures
	GameObject(const char* _ObjectFile, const char* _TexturePath[], int _TextureCount, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _ScreenWidth, int _ScreenHeight);

	//Object That use and .obj file
	GameObject(std::shared_ptr<Shader> _objectShader, std::shared_ptr<CameraObject> _camera, const char * _ObjectFile, int _ScreenWidth, int _ScreenHeight);

	
	~GameObject();

	//Set Rotation
	void setRotation(float _degree, glm::vec3 _rotation);

	//Set Position
	void setPosition(glm::vec3);

	//Set Scale
	void setScale(glm::vec3 _scale);
	
	//Set Directional Light Position
	void setDirLightPos(std::shared_ptr<GameObject> _lightSrc);
	//Set Point Light Positions and Amount
	void setPointLightPos(std::vector<std::shared_ptr<GameObject>> _lightPositions, int _AmountOfLights);

	//Get Position
	glm::vec3 getPosition();
	//Get Model Matrix
	glm::mat4 getModelMatrix();

private:

	
};
