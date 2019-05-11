//					MAIN PROGRAM OF LEARNING OpenGL 
//							BERTIN ROBIN
//					   	TRY TO DO HIS BEST

//------------------------------------------------------------------ 
// INCLUDE
//------------------------------------------------------------------ 

//Standard library
#include <iostream>
//OpenGL loader
#include <glad/glad.h> 
//GLFW library
#include <GLFW/glfw3.h>
//mathematical OpenGL library
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//library to open image in char
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Own Header
//Header file for shader class & build.
#include "shader.h"
#include "Camera.h"

//------------------------------------------------------------------ 
//Global Variables
//------------------------------------------------------------------ 


//Screen Variables
//------------------------------------------------------------------ 
const int screenHeight = 600;
const int screenWidth = 800;


//Camera Variables
//------------------------------------------------------------------ 

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;


//Time Variables 
//------------------------------------------------------------------ 

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Calling
//------------------------------------------------------------------ 
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//------------------------------------------------------------------ 


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
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);

	//Checks
	//------------------------------------------------------------------
	//Window check
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Configuration of Glfw
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide the cursor & enable catch mouse
	glfwSetCursorPosCallback(window, mouse_callback); //Save mouse movements
	glfwSetScrollCallback(window, scroll_callback); //save scroll movements

	//Glad check
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//Initialize the shader
	Shader object("object_v.vert", "object_f.vert");
	Shader lightingShader("lightning_v.vert", "lightning_f.vert");

	//SET vertices & buffer
	//------------------------------------------------------------------

	//Triangle
	float vertices3D[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//------------------------------------------------------------------ 
	//Initialize VAO, VBO, EBO
	//------------------------------------------------------------------ 


	//Creation of buffer  for object 
	//------------------------------------------------------------------

	//id of buffer & array 
	unsigned int VBO, VAO;
	//Buffer generation : only 1
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//Bind the VAO 
	glBindVertexArray(VAO);

	//link GL_ARRAY_BUFFER to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//save vertices in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesct3D), verticesct3D, GL_STATIC_DRAW);


	//Attribute pointers to buff to VAO  : position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	object.use();


	//Creation of buffer fot light
	//------------------------------------------------------------------ 
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	//Multiple cube Position.
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glm::vec3 lightPosition(1.2f, 1.0f, -1.0f);
	//------------------------------------------------------------------ 
	//RENDERING
	//------------------------------------------------------------------

	//Window size
	glViewport(0, 0, 800, 600);
	while (!glfwWindowShouldClose(window))
	{
		// Input
		//------------------------------------------------------------------ 
		processInput(window);

		// Rendering
		//------------------------------------------------------------------ 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculating the time of frame
		//------------------------------------------------------------------ 
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//------------------------------------------------------------------ 
		//SCENE, CAMERA & OBJECTS
		//------------------------------------------------------------------ 

		object.use();
		glBindVertexArray(VAO);

		// Camera 
		//------------------------------------------------------------------ 
		/*float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		glm::vec3 cameraPos = glm::vec3(camX, 0.0f, camZ);*/

		glm::mat4 view;
		view = camera.GetViewMatrix();
		//perpective matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		object.setMat4("view", view);
		object.setMat4("projection", projection);



		//Creation of object
		//------------------------------------------------------------------ 

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);
		object.setMat4("model", model);
		object.setVec3("lightColor", glm::vec3(1.0f));
		object.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		lightingShader.use();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe mode
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		//------------------------------------------------------------------ 
		//swap the buffer & check events
		//------------------------------------------------------------------ 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//------------------------------------------------------------------ 
	// OUT OF RENDERING LOOP
	//------------------------------------------------------------------ 


	// optional: de-allocate all resources once they've outlived their purpose:
	// -----------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}


// Input function to interact with the windows 
void processInput(GLFWwindow *window)
{	
	//close the windows when escape key is press
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Camera 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
}


// Save mouse position & enable the camera to rotate
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//save the scroll of mouse & enable zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}






// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}