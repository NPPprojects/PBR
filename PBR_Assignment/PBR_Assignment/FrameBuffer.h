#pragma once
#include "ObjectClass.h"
class FrameBuffer :
	public ObjectClass
{
public:

	FrameBuffer(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera,int _screenWidth,int _screenHeight);
	~FrameBuffer();
	void frameBufferConfiguration();
	int getFBO();
	void use();
	float setGamma(float _gamma);
	float setExposure(float _gamma);
  int setBlurIntesity(int _blurIntesity);
private:
	unsigned int frameBuffer;
	unsigned int textureColorBuffer;
	unsigned int renderBuffer;


  int blurIntesity;
	float exposure;
	float gamma;
};

