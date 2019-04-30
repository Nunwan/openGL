#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//shader source code in GLSC
const char *vertexShaderSource = "#version 330 core\n" //Version of OpenGL
"layout (location = 0) in vec3 aPos;\n" 
"layout (location = 1) in vec3 aColor; \n"
"out vec3 ourColor; \n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n" //Position of shader
"	ourColor = aColor;\n"
"}\0";
//fragment shader source code 
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0f);\n"
"}\0 ";

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
	//Set and compile shader : 
	//------------------------------------------------------------------

	//id of shader for vertex.
	unsigned int vertexShader;
	//vertexShader devient un shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//understanding the **	
	//std::cout << vertexShaderSource <<std::endl << &vertexShaderSource<<std::endl<<"3"<<std::endl;
	//link the vertexshader to the source code above
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//and compile
	glCompileShader(vertexShader);


	//compilation vertex  check 
	int  success;
	char infoLog[512];
	//compilation try 
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//print the error code if there are any.
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader 
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//compilation fragment check
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//link shaders 
	//------------------------------------------------------------------

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//linking check 
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//SET vertices & buffer
	//------------------------------------------------------------------

	//Triangle
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};
	//id of buffer & array 
	unsigned int VBO, VAO;
	//Buffer generation : only 1
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	
	//link GL_ARRAY_BUFFER to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//save vertices in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind the VAO 
	glBindVertexArray(VAO);
	//Attribute pointers to buff to VAO  : position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Attribute pointers to buff to VAO : color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




	//RENDERING
	//------------------------------------------------------------------

	glUseProgram(shaderProgram);
	//Window size
	glViewport(0, 0, 800, 600);
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// draw the triangle.
		
		glBindVertexArray(VAO);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe mode
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);	
		//swap the buffer & check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}