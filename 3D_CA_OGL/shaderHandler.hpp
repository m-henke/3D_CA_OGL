#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class shaderHandler {
	private:
		unsigned int shaderProgram;

		std::string parseShader(std::string path);
		void loadShader(unsigned int& shader, int shaderType, std::string path);
	public:
		shaderHandler(std::string vertPath, std::string fragPath);
		~shaderHandler();
		void use() const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif
