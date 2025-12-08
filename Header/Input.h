#pragma once
#include <GLFW/glfw3.h>

extern bool klimaOn;
extern float lampX, lampY, lampR;

void HandleLampClick(GLFWwindow* window);
void HandleKeyboardInput(GLFWwindow* window);
