#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Model {
	public:
		Model(std::vector<float> positions, std::vector<float> textures, std::vector<uint32_t> indices)
			: p_vertexCount{ indices.size() } {
			glGenVertexArrays(1, &this->p_vaoID);
			glBindVertexArray(this->p_vaoID);
			bindIndexBuffer(indices);
			storeDataIntoAttribArray(0, 3, positions);
			storeDataIntoAttribArray(1, 2, textures);
		}
	private:
		void bindIndexBuffer(std::vector<uint32_t> indices) {
			uint32_t eboID;
			glGenBuffers(1, &eboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices.front(), GL_STATIC_DRAW);
		}
		void storeDataIntoAttribArray(uint32_t attributeNumber, uint32_t coordinateSize, std::vector<float> data) {
			uint32_t vboID;
			glGenBuffers(1, &vboID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	public:
		uint32_t p_vaoID;
		uint64_t p_vertexCount;
};

class Texture {
	public:
		Texture(std::string textureName, uint32_t textureIndex) {
			if (textureIndex > 31) {
				std::cout << "TEXTURE INDEX CANNOT BE GREATER THAN 31!\nTEXTURE NAME: " << textureName << "\n";
			}
			glActiveTexture(GL_TEXTURE0 + textureIndex);
			glGenTextures(1, &this->m_textureID);
			glBindTexture(GL_TEXTURE_2D, this->m_textureID);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load and generate the texture
			int width, height, nrChannels;
			unsigned char *data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
			    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			    glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
			    std::cout << "Failed to load texture" << std::endl;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		}
		void bindTexture() {
			glBindTexture(GL_TEXTURE_2D, this->m_textureID);
		}
		void unbindTexture() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	private:
		uint32_t m_textureID;
};

class Transform {
	public:
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
			this->p_modelMatrix = glm::mat4(1);
			this->p_modelMatrix = glm::translate(this->p_modelMatrix, position);
			this->p_modelMatrix = glm::rotate(this->p_modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			this->p_modelMatrix = glm::rotate(this->p_modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
			this->p_modelMatrix = glm::rotate(this->p_modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			this->p_modelMatrix = glm::scale(this->p_modelMatrix, scale);
		}
	public:
		glm::mat4 p_modelMatrix;
};

class Shader {
	public:
		Shader(Transform transform, std::string vertexFile, std::string fragmentFile) 
			: m_transform{ transform } {
			this->m_vertexShaderID = compileShader(vertexFile, GL_VERTEX_SHADER);
			this->m_fragmentShaderID = compileShader(fragmentFile, GL_FRAGMENT_SHADER);
			this->p_shaderProgramID = glCreateProgram();
			glAttachShader(this->p_shaderProgramID, this->m_vertexShaderID);
			glAttachShader(this->p_shaderProgramID, this->m_fragmentShaderID);
			bindAllAttributes();
			glLinkProgram(this->p_shaderProgramID);
			glDetachShader(this->p_shaderProgramID, this->m_vertexShaderID);
			glDetachShader(this->p_shaderProgramID, this->m_fragmentShaderID);
			int32_t success;
			char infoLog[1024];
			glGetProgramiv(this->p_shaderProgramID, GL_LINK_STATUS, &success);
			if (success == GL_FALSE) {
				glGetProgramInfoLog(this->p_shaderProgramID, 1024, NULL, infoLog);
				std::cout << "FAILED TO LINK SHADER PROGRAM!\n" << infoLog << "\n";
			}
			getAllUniformLocations();
		}
		void start() {
			glUseProgram(this->p_shaderProgramID);
			loadMatrix(location_modelMatrix, m_transform.p_modelMatrix);
		}
		void stop() {
			glUseProgram(0);
		}
	private:
		void bindAllAttributes() {
			glBindAttribLocation(this->p_shaderProgramID, 0, "position");
			glBindAttribLocation(this->p_shaderProgramID, 1, "textureCoords");
		}
		uint32_t compileShader(std::string shaderFilePath, GLenum shaderType) {
			uint32_t shaderID = glCreateShader(shaderType);
			std::ifstream file(shaderFilePath);
			std::ostringstream ss;
			ss << file.rdbuf();
			std::string shaderCode = ss.str();
			const char* shaderSource = shaderCode.c_str();
			glShaderSource(shaderID, 1, &shaderSource, NULL);
			glCompileShader(shaderID);
			int32_t success;
			char infoLog[1024];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE) {
				glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
				if (shaderType == GL_VERTEX_SHADER) {
					std::cout << "FAILED TO COMPILE VERTEX SHADER!\n" << infoLog << "\n";
				}
				if (shaderType == GL_FRAGMENT_SHADER) {
					std::cout << "FAILED TO COMPILE FRAGMENT SHADER!\n" << infoLog << "\n";
				}
			}
			//std::cout << shaderSource << "\n";
			return shaderID;
		}
		void getAllUniformLocations() {
			location_modelMatrix = glGetUniformLocation(this->p_shaderProgramID, "modelMatrix");
		}
		void loadMatrix(uint32_t matrixLocation, glm::mat4 matrix) {
			glUniformMatrix4fv(matrixLocation, 1, false, glm::value_ptr(matrix));
		}
	public:
		uint32_t p_shaderProgramID;
	private:
		uint32_t m_vertexShaderID;
		uint32_t m_fragmentShaderID;
	private:
		uint32_t location_modelMatrix;
	private:
		Transform m_transform;
};

class Renderer {
	public:
		void prepare(glm::vec4 backgroundColor) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		}
		void render(Model model) {
			glBindVertexArray(model.p_vaoID);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawElements(GL_TRIANGLES, model.p_vertexCount, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}
};

class Input {
	public:
		void processKeyboardInput(GLFWwindow* window) {
			if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
		}
};

int main(void) {
	
	GLFWwindow* window;

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "FAILED TO INIT GLFW!\n";
		return -1;
	}

	window = glfwCreateWindow(800, 600, "OpenGL Rendering Port", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "FAILED TO CREATE WINDOW!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
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
	Transform transform(glm::vec3(-0.2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	Texture texture("../res/textures/grass.jpg", 0);
	Shader shader(transform, "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
	Renderer renderer;

	Input input;

	while (glfwWindowShouldClose(window) == GLFW_FALSE) {
		input.processKeyboardInput(window);
		renderer.prepare(glm::vec4(1, 0, 0, 1));
		shader.start();
		texture.bindTexture();
		renderer.render(model);
		texture.unbindTexture();
		shader.stop();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	std::cout << "UPLOAD TO GITHUB!!!!!!!!\n";
	return 0;
}
