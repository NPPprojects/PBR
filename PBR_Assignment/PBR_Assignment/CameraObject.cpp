#include "CameraObject.h"



CameraObject::CameraObject()
{
  Position = glm::vec3(0.0f, 0.0f, 3.0f);
  WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Yaw = YAW;
  Pitch = PITCH;
  Front = (glm::vec3(1.0f, 0.0f, -1.0f));
  MovementSpeed = SPEED;
  MouseSensitivity = SENSITIVITY;
  Zoom = ZOOM;
  updateCameraVectors();
}


CameraObject::~CameraObject()
{
}

glm::mat4 CameraObject::GetViewMatrix()
{
  return glm::lookAt(Position, Position + Front, Up);
}
void CameraObject::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    Position += Front * velocity;
  if (direction == BACKWARD)
    Position -= Front * velocity;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
}
void CameraObject::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}


void CameraObject::ProcessMouseScroll(float yoffset)
{
  if (Zoom >= 1.0f && Zoom <= 45.0f)
    Zoom -= yoffset;
  if (Zoom <= 1.0f)
    Zoom = 1.0f;
  if (Zoom >= 45.0f)
    Zoom = 45.0f;
}
void CameraObject::updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
}

float CameraObject::Getzoom() const
{
  return Zoom;
}

glm::vec3 CameraObject::GetPosition()
{
  return Position;
}

int CameraObject::getYaw() 
{
  return Yaw;
}
int CameraObject::getPitch()
{
  return Pitch;
}