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
		Tick();
		Render();
		glfwPollEvents();
	}

	void Tick() {
		ProcessInput();
	}

	void Render() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		constexpr float vertices[]{
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f
		};

		constexpr unsigned int indices[] = {
			0, 1, 2,
			1, 3, 2,
		};

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
		glEnableVertexAttribArray(0);

		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		const Shader shader{};
		shader.Bind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
