#include "engine.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/graphics.h"

namespace Engine {
	GLFWwindow *Window;

	int Initialize(const int windowWidth, const int windowHeight,
	               const char *windowTitle) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle,
		                          nullptr, nullptr);
		if (Window == nullptr) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(Window);

		if (!gladLoadGLLoader(
			reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glViewport(0, 0, windowWidth, windowHeight);
		glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
		return 0;
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(Window);
	}

	void Update() {
		glfwPollEvents();
		Tick();
		Render();
	}

	void Tick() {
		ProcessInput();
	}

	void Render() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const std::vector vertices{
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
		};

		const std::vector<unsigned int> indices = {
			0, 1, 2,
			1, 3, 2,
		};

		const Graphics::Shader shader{};

		Graphics::RenderObject obj(vertices, indices);
		obj.Render(shader);

		glfwSwapBuffers(Window);
	}

	void Terminate() {
		glfwTerminate();
	}

	void ProcessInput() {
		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(Window, true);
		}
	}

	void FramebufferSizeCallback(GLFWwindow *window, const int width,
	                             const int height) {
		glViewport(0, 0, width, height);
	}
} // namespace Engine
