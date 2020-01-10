#include "Scene.h"

int cases = 0;

void Scene::initalise(GLFWwindow* _window, std::shared_ptr<CameraObject> _camera)
{
	//Skybox
	cubeVAO = 0;
	cubeVBO = 0;

	//Camera
	FPScamera= _camera;
	//window
	window = _window;
	//GameTimeON
	spin=true;
	//Blinn Shading set to true by default
	blinn = true;
	blinnKeyPressed = true;
	//Shader Program
	//TextureShader3D = std::make_shared<Shader>("3DObjectShader.vert", "TextureShader.frag");
	ColorBoxShader = std::make_shared<Shader>("ColorBox.vert", "ColorBox.frag");
	LightBoxShader = std::make_shared<Shader>("LightBox.vert", "LightBox.frag");
	nanosuitShader = std::make_shared<Shader>("MultipleLights.vert", "MultipleLights.frag");
	frameBufferShader = std::make_shared<Shader>("framebufferScreen.vert", "framebufferScreen.frag");
	
	//cubeMapSkyboxShader = std::make_shared<Shader>("cubemapSkybox.vert", "cubemapSkybox.frag");
	//Phongg Lighting
	BPShader = std::make_shared<Shader>("Blinn-Phong.vert", "Blinn-Phong.frag");
	//Phongg Lighting Textured
	BPShaderTextured = std::make_shared<Shader>("Blinn-PhongTextured.vert", "Blinn-PhongTextured.frag");
	//Point Light PBR Shader
	PBRShader = std::make_shared<Shader>("PBR.vert", "PBR.frag");;
	//Point Light PBR Textured Shader
	PBRShaderTextured = std::make_shared<Shader>("PBRTextured.vert", "PBRTextured.frag");
	//IBL Lighting
	equirectangularShader = std::make_shared<Shader>("EquirectangularCubemap.vert", "EquirectangularCubemap.frag");
	backgroundShader = std::make_shared<Shader>("background.vert", "background.frag");

	//Objects

	LightBox = std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera, ScreenWidth, ScreenHeight);
	const char *textures[] = { "resources/textures/BetterBox.png" ,"resources/textures/BetterBox_spec.png"};
	textureCube = std::make_shared<GameObject>("ColorBox.data", textures,2, BPShaderTextured, FPScamera, ScreenWidth, ScreenHeight);
	for (int i = 0; i <= 1; i++)
	{
		lightBoxes.push_back(std::make_shared<GameObject>("LightBox.data", LightBoxShader, FPScamera, ScreenWidth, ScreenHeight));
	}

	

	//Blinn-Phong Spheres
	spheresBP = std::make_shared<SphereClass>(BPShader, FPScamera, ScreenWidth, ScreenHeight);
	spheresBP->initialiseSphere();
	//Blinn-Phong Spheres Textured
	spheresBPTextured = std::make_shared<SphereClass>(BPShaderTextured, FPScamera, ScreenWidth, ScreenHeight);
	spheresBPTextured->initaliseTextureSphere();
	//PBR Spheres
	spheresPBR = std::make_shared<SphereClass>(PBRShader, FPScamera, ScreenWidth, ScreenHeight);
	spheresPBR->initialiseSphere();
	//Textured PBR Spheres
	texturedSpheresPBR = std::make_shared<SphereClass>(PBRShaderTextured, FPScamera, ScreenWidth, ScreenHeight);
	texturedSpheresPBR->initaliseTextureSphere();


	frameBuffer = std::make_shared<FrameBuffer>("framebufferScreen.data", frameBufferShader, FPScamera, ScreenWidth, ScreenHeight);

	//Point Light positions
	
	positions.push_back(glm::vec3(1.5409f, 0.248259f, 0.822887f));
	positions.push_back(glm::vec3(-1.36048f, 0.248259f, 0.822887f));
	//Textures For rusting ball

	PBRShaderTextured->use();
	PBRShaderTextured->setInt("albedoMap", 0);
	PBRShaderTextured->setInt("normalMap", 1);
	PBRShaderTextured->setInt("metallicMap", 2);
	PBRShaderTextured->setInt("roughnessMap", 3);
	PBRShaderTextured->setInt("aoMap", 4);




	albedo = loadTexture("resources/textures/rusted/rust_basecolor.png");
	normal = loadTexture("resources/textures/rusted/rust_normal.png");
	metallic = loadTexture("resources/textures/rusted/rust_metallic.png");
	roughness = loadTexture("resources/textures/rusted/rust_roughness.png");	
	ao = loadTexture("resources/textures/rusted/ao.png");

	diffuse = loadTexture("resources/textures/BetterBox.png");
	specular = loadTexture("resources/textures/rusted/rust_metallic_inverted.png");

	PBRShader->use();
	PBRShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
	PBRShader->setFloat("ao", 1.0f);



	BPShader->use();

	lightColors.push_back(glm::vec3(150.0f, 150.0f, 150.0f));
	lightColors.push_back(glm::vec3(150.0f, 150.0f, 150.0f));

	BPShader->use();
	BPShader->setVec3("objectColor",glm::vec3(0.5f, 0.0f, 0.0f));
	BPShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	//FrameBuffer for HDR Map set up

	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// pbr: load the HDR environment map
	// ---------------------------------
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(("resources/textures/hdr/Newport_Loft_Ref.hdr"), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	equirectangularShader->use();
	equirectangularShader->setInt("equirectangularMap", 0);
	equirectangularShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//equirectangularCube = std::make_shared<ObjectClass>("resources/objects/primitives/equirectangularcube.data", equirectangularShader, FPScamera, ScreenWidth, ScreenHeight);
		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(FPScamera->Getzoom()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);

	backgroundShader->use();
	backgroundShader->setMat4("projection", projection);

	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
	//Render Loop
	
}

void Scene::loadSceneOne()
{

	int frameCount = 0;
	
	double previousFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		/*
		glm::vec3 tracker = FPScamera->GetPosition();
		std::cout << tracker.x << "  " << tracker.y << "  " << tracker.z << ": WORLD POSITIONS" << std::endl;
		std::cout << "Up/Down: " << FPScamera->getPitch() << "      " << "Left//Right: " << FPScamera->getYaw() << std::endl;
		std::cout << FPScamera->getPitch() << "Left//Right" << std::endl;
		Timer
		std::cout << glfwGetTime()<<std::endl;
		*/
		double currentFrame = glfwGetTime();
		frameCount++;
		if (currentFrame - previousFrame >= 1.0)
		{
			// Output FrameRate
			//printf("%f ms/frame\n", 1000.0 / double(frameCount));
			std::cout << frameCount<<std::endl;
			frameCount = 0;
			previousFrame += 1.0;
		}
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//http://www.opengl-tutorial.org/miscellaneous/an-fps-counter/ 

		//Input handling
		if (spin == true)
		{
			//	std::cout << "Spinning" << std::endl;
			Time = glfwGetTime();
		}
		else if (spin == false)
		{
			//	std::cout << "Paused" << std::endl;
			glfwSetTime(Time);
			deltaTime = 0.02f;
		}
		processInput(window, frameBuffer);

		
		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getFBO());
		
		glClearColor(0.184f, 0.196f, 0.235f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		glEnable(GL_DEPTH_TEST); // enable depth testing (is di sabled for rendering screen-space quad)
	
		glm::mat4 view = FPScamera->GetViewMatrix();

		for (int i = 0; i <= 1; i++)
		{
			lightBoxes.at(i)->use3D();
			lightBoxes.at(i)->setPosition(positions[i]);

			lightBoxes.at(i)->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		}
	
		positions.at(0) = glm::vec3(10.0f * sin(Time), 7.0f, 10.0f * cos(Time));	
		positions.at(1) = glm::vec3(-10.0f * sin(Time), -7.0f, -10.0f * cos(Time));
	

		

		//Blinn-Phong Spheres

		switch (cases)
		{
			case 1:
			{
				spheresBP->useSphere();
				BPShader->use();

				glm::vec3 lightColor = glm::vec3(100.0f, 100.0f, 100.0f);
				glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
				glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	

				for (unsigned int i = 0; i <= 1; ++i)
				{
					BPShader->setVec3("pointLights[" + std::to_string(i) + "].position", lightBoxes.at(i)->getPosition());
					BPShader->setBool("pointLights[" + std::to_string(i) + "].blinn", blinn);
					BPShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", ambientColor);
					BPShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuseColor);
					BPShader->setVec3("pointLights[" + std::to_string(i) + "].specular", 10.0f, 10.0f, 10.0f);

					//Point light attenuation
					BPShader->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
					BPShader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.045f);
					BPShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.075f);
				}

				// material properties red plastic
				BPShader->setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
				BPShader->setVec3("material.diffuse", 0.5f, 0.0f, 0.0f);
				BPShader->setVec3("material.specular", 0.7, 0.6f, 0.6f); // specular lighting doesn't have full effect on this object's material
				BPShader->setFloat("material.shininess", 64);
				break;
			}
		//Write a Blinn-Phongg model for textures
			case 2:
			{
				glm::vec3 lightColor = glm::vec3(10.0f, 10.0f, 10.0f);
				glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
				glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
			
				BPShaderTextured->use();
				BPShaderTextured->setInt("material.diffuse", 0);
				BPShaderTextured->setInt("material.specular", 1);
		
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, albedo);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, specular);
				spheresBPTextured->useTextureSphere();
				
				for (unsigned int i = 0; i <= 1; ++i)
				{
					
					
					BPShaderTextured->setVec3("pointLights[" + std::to_string(i) + "].position", lightBoxes.at(i)->getPosition());
					BPShaderTextured->setBool("pointLights[" + std::to_string(i) + "].blinn", blinn);
					BPShaderTextured->setVec3("pointLights[" + std::to_string(i) + "].ambient", ambientColor);
					BPShaderTextured->setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuseColor);
					BPShaderTextured->setVec3("pointLights[" + std::to_string(i) + "].specular", 10.0f, 10.0f, 10.0f);

					//Point light attenuation
					BPShaderTextured->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
					BPShaderTextured->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.045f);
					BPShaderTextured->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.075f);

					
			
					
					BPShaderTextured->setFloat("material.shininess", 32);
				};
				
				break;
			}
			//PBR Spheres
			case 3:
			{
				spheresPBR->useSphere();
				for (unsigned int i = 0; i <= 1; ++i)
				{
					PBRShader->use();
					PBRShader->setVec3("lightPositions[" + std::to_string(i) + "]", lightBoxes.at(i)->getPosition());
					PBRShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors.at(i));
					
				};
				break;
			}
			//spheresPBR->useSphere();
			//texturedSpheresPBR->useTextureSphere();

			case 4:
			{

				
				glActiveTexture(GL_TEXTURE0);
 				glBindTexture(GL_TEXTURE_2D, albedo);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normal);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, metallic);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, roughness);
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, ao);
				texturedSpheresPBR->useTextureSphere();
				for (unsigned int i = 0; i <= 1; ++i)
				{
					PBRShaderTextured->setVec3("lightPositions[" + std::to_string(i) + "]", lightBoxes.at(i)->getPosition());
					PBRShaderTextured->setVec3("lightColors[" + std::to_string(i) + "]", lightColors.at(i));

				}
				break;
			}
			default:
				std::cout << "scene is empty" << std::endl;
		}
		//skybox->use();

		backgroundShader->use();
		backgroundShader->setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		//equirectangularCube->use3D();
		
		renderCube();

		//now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		//clear all relevant buffers
		glClearColor(0.184f, 0.196f, 0.235f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//use the color attachment texture as the texture of the quad plane

		frameBuffer->use();






		//swap buffers and poll IO events key pressed/released, mosuse moved etc.
		glfwSwapBuffers(window);
		glfwPollEvents();

		//glfw: terminate, clearning all previously allocated GLFW resources

	}



	//EmissionBoxes.clear();
	//lightBoxes.clear();
	glfwTerminate();
	//delete nanosuitTest;*/

}

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::processInput(GLFWwindow * window, std::shared_ptr<FrameBuffer> _framebuffer)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		FPScamera->ProcessKeyboard(FPScamera->RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		spin = false;	
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		spin = true;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		_framebuffer->setGamma(0.01);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		_framebuffer->setGamma(-0.01);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		_framebuffer->setExposure(0.01);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		_framebuffer->setExposure(-0.01);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		_framebuffer->setFilter(true);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		_framebuffer->setFilter(false);
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		blinn = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		blinn = false;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		cases = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		cases = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		cases = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		cases = 4;
	}
	
	
}

int Scene::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Scene::renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
