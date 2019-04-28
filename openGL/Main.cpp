#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//shader source code in GLSC
const char *vertexShaderSource = "#version 330 core\n" //Version of OpenGL
"layout (location = 0) in vec3 aPos;\n" 
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" //Position of shader
"}\0";
//fragment shader source code 
const char *fragmentShaderSourceRouge = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0 ";

const char *fragmentShaderSourceGreen = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
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

	//fragment shader rouge
	unsigned int fragmentShaderRouge;
	fragmentShaderRouge = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRouge, 1, &fragmentShaderSourceRouge, NULL);
	glCompileShader(fragmentShaderRouge);


	//compilation fragment check rouge
	glGetShaderiv(fragmentShaderRouge, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderRouge, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader  vert
	unsigned int fragmentShaderGreen;
	fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderGreen, 1, &fragmentShaderSourceGreen, NULL);
	glCompileShader(fragmentShaderGreen);


	//compilation fragment check vert
	glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderGreen, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//link shaders 
	//------------------------------------------------------------------
	//Rouge
	unsigned int shaderProgramRouge;
	shaderProgramRouge = glCreateProgram();
	glAttachShader(shaderProgramRouge, vertexShader);
	glAttachShader(shaderProgramRouge, fragmentShaderRouge);
	glLinkProgram(shaderProgramRouge);

	//linking check 
	glGetProgramiv(shaderProgramRouge, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramRouge, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Vert
	unsigned int shaderProgramGreen;
	shaderProgramGreen = glCreateProgram();
	glAttachShader(shaderProgramGreen, vertexShader);
	glAttachShader(shaderProgramGreen, fragmentShaderGreen);
	glLinkProgram(shaderProgramGreen);

	//linking check 
	glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderRouge);
	glDeleteShader(fragmentShaderGreen);
	//SET vertices & buffer
	//------------------------------------------------------------------

	//Triangle
	float triangle1[] = {
		//triangle 1
		0.5f, -0.5f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,  // bottom left
	   -0.5f,  0.5f, 0.0f,   // top left 
	};
	float triangle2[] = {
		//triangle 2
		0.5f,  0.5f, 0.0f,  // top right
		-0.5f,  0.5f, 0.0f,  // top left 
		0.5f, -0.5f, 0.0f  // bottom right

	};

	/* unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	}; */
	//id of buffer & array 
	unsigned int VBO[2], VAO[2];
	// unsigned int EBO;
	//Buffer generation : only 1
	glGenBuffers(2, VBO);
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(2, VAO);
	
	//link GL_ARRAY_BUFFER to VBO 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//save vertices in memory 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

	//Bind the VAO 1
	glBindVertexArray(VAO[0]);
	//Attribute pointers to buff to VAO 1
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//link GL_ARRAY_BUFFER to VBO2
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//save vertices in memory 2
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

	//Bind the VAO 2
	glBindVertexArray(VAO[1]);
	//Attribute pointers to buff to VAO  2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//BIND EBO 
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); */



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
		// draw the triangle.
		glUseProgram(shaderProgramRouge);
		glBindVertexArray(VAO[0]);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe mode
		glDrawArrays(GL_TRIANGLES, 0,3);
		glBindVertexArray(0);
		//triangle 2 
		glUseProgram(shaderProgramGreen);
		glBindVertexArray(VAO[1]);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe mode
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//swap the buffer & check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}