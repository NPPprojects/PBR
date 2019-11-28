#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(const char* _ObjectFile, std::shared_ptr <Shader> _objectShader, std::shared_ptr <CameraObject> _camera, int _screenWidth, int _screenHeight) :
	ObjectClass(_ObjectFile, _objectShader, _camera)
{
	 screenWidth = _screenWidth;
	 screenHeight = _screenHeight;
	frameBufferConfiguration();
	gamma = 1.0 / 2.0;
}

FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::frameBufferConfiguration()
{
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// create a color attachment texture

	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer); // now actually attach it
	//Error Handling
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int FrameBuffer::getFBO()
{
	return frameBuffer;
}

void FrameBuffer::use()
{
	objectShader->use();
	glBindVertexArray(getVAO());
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, verteciesCount);

	objectShader->setFloat("gamma", gamma);
}

float FrameBuffer::setGamma(float _gamma)
{
	return gamma += _gamma;
}