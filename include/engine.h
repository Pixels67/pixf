#pragma once

struct GLFWwindow;

namespace engine {
int Initialize(int windowWidth, int windowHeight, const char* windowTitle);

bool ShouldClose();

void Update();

void Tick();

void Render();

void Terminate();

void ProcessInput();

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
}  // namespace engine
