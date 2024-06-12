#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderHandler.hpp"

class Renderer {
	private:
		const unsigned int width, height;

	public:
		GLFWwindow* window;

		Renderer(const unsigned int w, const unsigned int h, const char* title);
		~Renderer();
		void draw();
};

#endif // !RENDERER_H
