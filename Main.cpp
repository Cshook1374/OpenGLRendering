#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Rendering Port"

#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

class Model {
	public:
		Model(std::vector<float> positions, std::vector<float> textures, std::vector<uint32_t> indices)
			: m_vertexCount{ indices.size() } {
			glGenVertexArrays(1, &this->m_vaoID);
			glBindVertexArray(this->m_vaoID);
			bindIndexBuffer(indices);
			storeDataIntoAttributeList(0, 3, positions);
			storeDataIntoAttributeList(1, 2, textures);
		}
	public:
		uint32_t& vaoID() {
			return this->m_vaoID;
		}
		uint64_t& vertexCount() {
			return this->m_vertexCount;
		}
	private:
		void bindIndexBuffer(std::vector<uint32_t> indices) {
			uint32_t vboID;
			glGenBuffers(1, &vboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices.front(), GL_STATIC_DRAW);	
		}
		void storeDataIntoAttributeList(uint32_t attributeNumber, uint32_t coordinateSize, std::vector<float> data) {
			uint32_t vboID;
			glGenBuffers(1, &vboID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	private:
		uint32_t m_vaoID;
		uint64_t m_vertexCount;
};

class Transform {
	public:
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
			this->m_position = position;
			this->m_rotation = rotation;
			this->m_scale = scale;
			this->m_modelMatrix = glm::mat4(1);
			this->m_modelMatrix = glm::translate(this->m_modelMatrix, this->m_position);
			this->m_modelMatrix = glm::rotate(this->m_modelMatrix, this->m_rotation.x, glm::vec3(1, 0, 0));
			this->m_modelMatrix = glm::rotate(this->m_modelMatrix, this->m_rotation.y, glm::vec3(0, 1, 0));
			this->m_modelMatrix = glm::rotate(this->m_modelMatrix, this->m_rotation.z, glm::vec3(0, 0, 1));
			this->m_modelMatrix = glm::scale(this->m_modelMatrix, this->m_scale);
		}
	public:
		glm::mat4& modelMatrix() {
			return this->m_modelMatrix;
		}
	private:
		glm::mat4 m_modelMatrix;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
};

class Shader {
	public:
		Shader(Transform transform, std::string vertexFile, std::string fragmentFile)
			: m_transform{transform} {
			this->m_vertexShaderID	= compileShader(vertexFile, GL_VERTEX_SHADER);
			this->m_fragmentShaderID = compileShader(fragmentFile, GL_FRAGMENT_SHADER);
			this->m_shaderProgramID = glCreateProgram();
			glAttachShader(this->m_shaderProgramID, this->m_vertexShaderID);
			glAttachShader(this->m_shaderProgramID, this->m_fragmentShaderID);
			bindAllAttributes();
			glLinkProgram(this->m_shaderProgramID);
			int32_t success;
			char infoLog[1024];
			glGetProgramiv(this->m_shaderProgramID, GL_LINK_STATUS, &success);
			if (success == GL_FALSE) {
				std::cout << "FAILED TO LINK SHADER PROGRAM!\n" << infoLog << "\n";
			}
			getAllUniformLocations();
			glm::mat4 testMatrix;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					glGetUniformfv(this->m_shaderProgramID, location_modelMatrix, glm::value_ptr(testMatrix));
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << testMatrix[i][j] << "\n";
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << this->m_transform.modelMatrix()[i][j] << "\n";
				}
			}
		}
		void start() {
			glUseProgram(this->m_shaderProgramID);
			loadMatrix(this->location_modelMatrix, this->m_transform.modelMatrix());
		}
		void stop() {
			glUseProgram(0);
		}
	public:
		Transform& transform() {
			return this->m_transform;
		}
	private:
		void getAllUniformLocations() {
			glGetUniformLocation(this->location_modelMatrix, "modelMatrix");
		}
		void loadMatrix(uint32_t uniformLocation, glm::mat4 matrix) {
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
		}
		void bindAllAttributes() {
			glBindAttribLocation(this->m_shaderProgramID, 0, "positions");
			glBindAttribLocation(this->m_shaderProgramID, 1, "textures");
		}
		uint32_t compileShader(std::string shaderFilePath, GLenum shaderType) {
			uint32_t shaderID = glCreateShader(shaderType);
			std::ifstream file(shaderFilePath);
			std::ostringstream ss;
			ss << file.rdbuf();
			std::string shaderCode = ss.str();
			const char* shaderSource = shaderCode.c_str();
			glShaderSource(shaderID, 1, &shaderSource, nullptr);
			glCompileShader(shaderID);
			int32_t success;
			char infoLog[1024];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE) {
				glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
				if (shaderType == GL_VERTEX_SHADER)
					std::cout << "FAILED TO COMPILE VERTEX SHADER!\n" << infoLog << "\n";
				if (shaderType == GL_FRAGMENT_SHADER)
					std::cout << "FAILED TO COMILE FRAGMENT SHADER!\n" << infoLog << "\n";
			}
			return shaderID;
		}
	private:
		uint32_t location_modelMatrix;
	private:
		Transform m_transform;
		uint32_t m_vertexShaderID;
		uint32_t m_fragmentShaderID;
		uint32_t m_shaderProgramID;
};

class Texture {
	public:
		Texture(std::string textureFilePath, uint32_t textureIndex) {
			if (textureIndex > 31) {
				std::cout << "TEXTURE INDEX CANNOT BE GREATER THAN 31!\n";
				return;
			}
			glActiveTexture(35008 + textureIndex);
			glGenTextures(1, &this->m_textureID);
			glBindTexture(GL_TEXTURE_2D, this->m_textureID);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load and generate the texture
			int width, height, nrChannels;
			unsigned char *data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
			    std::cout << "FAILED TO LOAD TEXTURE RESOURCE! : " << textureFilePath << "\n";
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
	public:
		uint32_t& textureID() {
			return m_textureID;
		}
	private:
		uint32_t m_textureID;
};

class Renderer {
	public:
		void prepare(glm::vec4 background) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(background.r, background.g, background.b, background.a);
		}
		void render(Model model) {
			glBindVertexArray(model.vaoID());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}
};

int main(void) {
	GLFWwindow* window;

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "FAILED TO INITIALIZE GLFW! EXITIING WITH RETURN CODE -1!\n";
		return -1;
	}

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	
	if (window == nullptr) {
		std::cout << "FAILED TO CREATE GLFW WINDOW! EXITING WITH RETURN CODE -1!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "FAILED TO INITIALIZE GLEW! EXITIING WITH RETURN CODE -1!\n";
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
	Transform transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	Texture texture("../res/textures/image.jpg", 0);
	Shader shader(transform, "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
	Renderer renderer;

	while (glfwWindowShouldClose(window) == GLFW_FALSE) {

		// RENDER HERE
		renderer.prepare(glm::vec4(1, 0, 0, 1));
		shader.start();
		texture.bindTexture();
		renderer.render(model);
		texture.unbindTexture();
		shader.stop();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
