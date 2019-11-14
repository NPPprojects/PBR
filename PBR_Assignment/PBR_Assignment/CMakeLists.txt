cmake_minimum_required(VERSION 2.6)
project(REND)

add_executable(triangle
  CameraObject.cpp
  CustomCursor.cpp
  GameObject.cpp
  Main.cpp
  Mesh.cpp
  Model.cpp
  ObjectClass.cpp
  Shader.cpp
  stb_image.cpp
  stdafx.cpp
)
target_link_libraries(triangle GL GLEW glfw assimp)
