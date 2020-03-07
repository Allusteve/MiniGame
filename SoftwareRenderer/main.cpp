#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"



using namespace std;


const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
void processInput(GLFWwindow* window);

int main()
{
	// Window Init
	glfwInit();

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glm::mat4 ViewPortMatrix = Math::GetViewPortMatrix(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Vertex V1(glm::vec3(-0.5, -0.5, 0), glm::vec4(255, 0, 0, 0));
	Vertex V2(glm::vec3(0.5, -0.5, 0), glm::vec4(0, 255, 0, 0));
	Vertex V3(glm::vec3(0, 0.5, 0), glm::vec4(0, 0, 255, 0));

	Renderer& renderer = Renderer::Get();
	renderer.Init(SCR_WIDTH, SCR_HEIGHT);
	renderer.SetViewportMatrix(ViewPortMatrix);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderer.ClearColorBuffer(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		//ScanLineTriangle(o1, o2, o3);
		//glDrawPixels(SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, FrontBuffer.data());
		renderer.DrawTriangle(V1, V2, V3);
		renderer.Flush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

