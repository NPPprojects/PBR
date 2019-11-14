#pragma once
#include "stdafx.h"

class CameraObject
{
private:
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.1f;
  const float ZOOM = 45.0f;

public:
  enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };
  void updateCameraVectors();

  CameraObject();
  ~CameraObject();

  void ProcessMouseScroll(float);
  void ProcessMouseMovement(float, float, GLboolean);
  void ProcessKeyboard(Camera_Movement, float);
  glm::mat4 GetViewMatrix();
  float Getzoom() const;
  glm::vec3 GetPosition();
  int getYaw();
  int getPitch();
  // Constructor with scalar values
};

