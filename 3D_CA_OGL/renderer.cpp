#include "renderer.hpp"
#include "cube.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

Renderer::Renderer(const unsigned int w, const unsigned int h, const char* title) : width(w), height(h) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		exit(-1);
	}

	// configure opengl state
	glEnable(GL_DEPTH_TEST);

	// setup buffers
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// filled cube
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// wireframe cube
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wfCubeVertices), wfCubeVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load shaderprogram
	cubeShader = new shaderHandler("cubeShader.vert", "cubeShader.frag");

	// set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	cubeShader->use();
	cubeShader->setMat4("projection", projection);
}

Renderer::~Renderer() {
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	cubeShader->~shaderHandler();
	glfwTerminate();
}

void Renderer::draw(std::vector<cell> positions) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cubeShader->use();

	// camera/view transformation
	glm::mat4 view = glm::mat4(1.0f);
	float radius = 2.2f;
	float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
	view = glm::lookAt(glm::vec3(camX, 0.8f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cubeShader->setMat4("view", view);

	// render boxes
	glBindVertexArray(VAOs[0]);
	for (unsigned int i = 0; i < static_cast<int>(positions.size()); i++) {
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::translate(model, -positions[i].pos + 49.5f);
		cubeShader->setMat4("model", model);
	    glDrawArrays(GL_TRIANGLES, 0, cubeNumVertices);
	}

	// render wireframe boarder
	glBindVertexArray(VAOs[1]);
	cubeShader->setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_LINES, 0, wfCubeNumVertices);

	glfwSwapBuffers(window);
}
