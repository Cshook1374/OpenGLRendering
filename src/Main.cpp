#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Rendering Port"

#include <iostream>

int main(void) {
	GLFWwindow* window;

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "FAILED TO INIT GLFW!\n";
		return -1;
	}

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "FAILED TO INIT GLEW!\n";
		glfwTerminate();
		return -1;
	}

	std::vector<float> positions = {
		-0.5, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0
	};

	std::vector<float> textures = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	std::vector<uint32_t> indices = {
		0, 1, 3,
		3, 1, 2
	};

	Model model(positions, textures, indices);
	Renderer renderer;
	Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

	while (glfwWindowShouldClose(window) == GLFW_FALSE) {
		renderer.prepare(glm::vec4(1, 0, 0, 1));
		shader.start();
		renderer.render(model);
		shader.stop();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}