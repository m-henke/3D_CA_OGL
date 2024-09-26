#include "shaderHandler.hpp"

std::string shaderHandler::parseShader(std::string path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Failed to open file: " << path << '\n';
		exit(-1);
	}

	std::string ret;
	std::ostringstream ss;
	ss << file.rdbuf();
	ret = ss.str();

	file.close();
	return ret;
}

void shaderHandler::loadShader(unsigned int& shader, int shaderType, std::string path) {
	shader = glCreateShader(shaderType);
	std::string shaderSourceString = parseShader(path);
	const char* shaderSource = shaderSourceString.c_str();
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}
}

shaderHandler::shaderHandler(std::string vertPath, std::string fragPath) {
	unsigned int vertShader, fragShader;
	loadShader(vertShader, GL_VERTEX_SHADER, vertPath);
	loadShader(fragShader, GL_FRAGMENT_SHADER, fragPath);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

shaderHandler::~shaderHandler() {
	glDeleteProgram(shaderProgram);
}

void shaderHandler::use() const {
	glUseProgram(shaderProgram);
}

void shaderHandler::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shaderHandler::setVec3(const std::string& name, const glm::vec3& vec) const {
	glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
}

void shaderHandler::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}