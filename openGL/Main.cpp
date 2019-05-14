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
#include "Model.h"
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
double lastX = screenWidth / 2.0f;
double lastY = screenHeight / 2.0f;
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
unsigned int load_Texture(char const* path);
//------------------------------------------------------------------ 


int main()
{

	//------------------------------------------------------------------ 
	// Initialisation & Checks of Import Libraries
	//------------------------------------------------------------------ 


	
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

	//------------------------------------------------------------------ 
	// Creation of object & Shaders
	//------------------------------------------------------------------ 


	// Initialize the shader with an object & a light
	//------------------------------------------------------------------ 

	Shader ourShader("object_v.vert", "object_f.vert");
	Shader lightingShader("lightning_v.vert", "lightning_f.vert");

	//load Models
	//------------------------------------------------------------------ 
	Model ourModel("../resources/nanosuit/nanosuit.obj");

	
	//------------------------------------------------------------------ 
	//RENDERING
	//------------------------------------------------------------------

	//Window size
	glViewport(0, 0, 800, 600);
	while (!glfwWindowShouldClose(window))
	{
			
		// per-frame time logic
	    // --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		//------------------------------------------------------------------ 
		processInput(window);

		// Rendering
		//------------------------------------------------------------------ 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();

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
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		//Creation of object
		//------------------------------------------------------------------ 

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);





		


		//------------------------------------------------------------------ 
		//swap the buffer & check events
		//------------------------------------------------------------------ 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//------------------------------------------------------------------ 
	// OUT OF RENDERING LOOP
	//------------------------------------------------------------------ 


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

unsigned int load_Texture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}






// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}