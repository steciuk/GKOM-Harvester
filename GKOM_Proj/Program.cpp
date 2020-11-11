#define GLEW_STATIC
#include <GL/glew.h>
#include "headers/VertexBuffer.h"
#include "headers/VertexBufferLayout.h"
#include "headers/IndexBuffer.h"
#include "headers/VertexArray.h"
#include "headers/ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint LoadMipmapTexture(GLuint texId, const char* fname);

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

int main()
{
	/*{
		glm::mat4 trans;
		cout << trans << endl;
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		cout << trans << endl;
	}*/
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - Kombajn", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// Let's check what are maximum parameters counts
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		cout << "Max vertex attributes allowed: " << nrAttributes << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &nrAttributes);
		cout << "Max texture coords allowed: " << nrAttributes << std::endl;

		// Build, compile and link shader program
		ShaderProgram shaderProgram("shaders/vertexShader.vert", "shaders/fragmentShader.frag");

		// Set up vertex data 
		GLfloat vertices[] = {
			// coordinates			// color			// texture
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,

			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		};

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23,
		};

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		VertexArray vertexArray;								//VAO
		VertexBufferLayout layout;
		VertexBuffer vertexBuffer(vertices, sizeof(vertices));	//VBO
		IndexBuffer indicesBuffer(indices, sizeof(indices));	//EBO

		// vertex geometry data
		layout.addFloat(3);
		// vertex color data
		layout.addFloat(3);
		// vertex texture coordinates
		layout.addFloat(2);
		vertexArray.LinkBuffer(vertexBuffer, layout);

		vertexBuffer.Unbind();
		vertexArray.Unbind();

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "textures/container.jpg");
		GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "textures/weiti.png");

		glEnable(GL_DEPTH_TEST);

		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0);
			shaderProgram.setUniformInt("Texture0", 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			shaderProgram.setUniformInt("Texture1", 1);

			glm::mat4 trans;
			static GLfloat rot_angle = 15.0f;
			trans = glm::rotate(trans, (float)glfwGetTime() * glm::radians(rot_angle), glm::vec3(1.0, 0.3, 0.5));
			shaderProgram.setUniformMat4("transform", trans);

			// Draw our first box
			shaderProgram.Use();

			//glBindVertexArray(VAO);
			vertexArray.Bind();
			glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
		//glDeleteVertexArrays(1, &VAO);
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

/* Alternative to abstractions
GLuint VBO, EBO, VAO;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);

glGenBuffers(1, &VBO)
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// vertex geometry data
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);

// vertex color data
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);

// vertex texture coordinates
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);

glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
*/