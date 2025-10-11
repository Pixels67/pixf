#pragma once

struct GLFWwindow;

namespace pixf {
void Initialize(int window_width, int window_height, const char* window_title);

bool ShouldClose();

void Update();

void Tick();

void Render();

void Terminate();

void ProcessInput();

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
}  // namespace pixf
