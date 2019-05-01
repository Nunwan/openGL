#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Input function to close windows with escape 
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
int main()
{
	//Initialisation of glfw 
	//------------------------------------------------------------------

	glfwInit();
	//OpenGL Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//profile Core of openGL & glad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	

	//Window creation 
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	//Checks
	//------------------------------------------------------------------
	//Window check
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Glad check
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Initialize the shader
	Shader ourShader("vertex.vert", "fragment.vert");


	//SET vertices & buffer
	//------------------------------------------------------------------

	//Triangle
	float vertices[] = {
		// positions        
		 0.5f, -0.2f, 0.0f, // bottom right
		-0.5f, -0.2f, 0.0f, // bottom left
		 0.0f,  0.5f, 0.0f  // top 
	};
	float verticesc[] = {
		// positions         // colors
		 0.5f, -0.2f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.2f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};


	float verticesct[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	//indices for EBO 

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	//Texture
	//------------------------------------------------------------------
	
	// set gl texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load the image
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../include/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	

	stbi_image_free(data);


	
	//Creation of buffer 
	//------------------------------------------------------------------

	//id of buffer & array 
	unsigned int VBO, VAO, EBO;
	//Buffer generation : only 1
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Bind the VAO 
	glBindVertexArray(VAO);

	//link GL_ARRAY_BUFFER to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//save vertices in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesct), verticesct, GL_STATIC_DRAW);

	//Buffer EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	//Attribute pointers to buff to VAO  : position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);




	//RENDERING
	//------------------------------------------------------------------

	//Window size
	glViewport(0, 0, 800, 600);
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//others

		// draw the triangle.
		glBindTexture(GL_TEXTURE_2D, texture);
		ourShader.use();

		glBindVertexArray(VAO);

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe mode

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//	glDrawArrays(GL_TRIANGLES, 0, 3);	

		//swap the buffer & check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}