#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;


const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
void processInput(GLFWwindow* window);
glm::vec3 eye(0, 3, 3);

//输入四个点坐标，分别是左上 左下 右下 右上 和法线方向
Mesh CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal) {
	Mesh result(4, 6);
	result.VertexBuffer[0].position = glm::vec4(leftTop, 1.0f);
	result.VertexBuffer[0].normal = normal;
	result.VertexBuffer[0].color = glm::vec4(255, 0, 0, 255);
	result.VertexBuffer[0].texCoords = glm::vec2(0.0f, 1.0f);
	result.VertexBuffer[1].position = glm::vec4(rightTop, 1.0f);
	result.VertexBuffer[1].normal = normal;
	result.VertexBuffer[1].color = glm::vec4(0, 255, 0, 255);
	result.VertexBuffer[1].texCoords = glm::vec2(1.0f, 1.0f);
	result.VertexBuffer[2].position = glm::vec4(rightBottom, 1.0f);
	result.VertexBuffer[2].normal = normal;
	result.VertexBuffer[2].color = glm::vec4(0, 0, 255, 0);
	result.VertexBuffer[2].texCoords = glm::vec2(1.0f, 0.0f);
	result.VertexBuffer[3].position = glm::vec4(leftBottom, 1.0f);
	result.VertexBuffer[3].normal = normal;
	result.VertexBuffer[3].color = glm::vec4(255, 0, 255, 255);
	result.VertexBuffer[3].texCoords = glm::vec2(0.0f, 0.0f);
	//绘制三角形的顺序是 左上->右下->右下 左上->左下->右上 都是逆时针方向 
	result.IndexBuffer[0] = 0;
	result.IndexBuffer[1] = 2;
	result.IndexBuffer[2] = 1;
	result.IndexBuffer[3] = 0;
	result.IndexBuffer[4] = 3;
	result.IndexBuffer[5] = 2;
	return result;
}

Mesh CreateBox(const glm::vec3& center, float radius) {
	Mesh result;
	Mesh front = CreatePlane(
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, radius, radius),
		glm::vec3(0, 0, 1)
	);
	result.AddMesh(front);

	Mesh left = CreatePlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, radius, radius),
		glm::vec3(-1, 0, 0)
	);
	result.AddMesh(left);

	Mesh right = CreatePlane(
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(1, 0, 0)
	);
	result.AddMesh(right);

	Mesh back = CreatePlane(
		center + glm::vec3(radius, radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, radius, -radius),
		glm::vec3(0, 0, -1)
	);
	result.AddMesh(back);

	Mesh up = CreatePlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(0, 1, 0)
	);
	result.AddMesh(up);

	Mesh down = CreatePlane(
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, -radius, radius),
		glm::vec3(0, -1, 0)
	);
	result.AddMesh(down);

	return result;
}

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
	//glm::mat4 ProjectionMatrix = Math::GetProjectionMatrix(glm::radians(60.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.3f, 100);
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH/SCR_HEIGHT	, 0.5f, 100.0f);
	

	Vertex V1(glm::vec3(-0.5, -0.5, 0), glm::vec4(255, 0, 0, 0));
	Vertex V2(glm::vec3(0.5, -0.5, 0), glm::vec4(0, 255, 0, 0));
	Vertex V3(glm::vec3(0, 0.5, 0), glm::vec4(0, 0, 255, 0));

	Mesh Box = CreateBox(glm::vec3(0.0, 0.0, 0.0), 1);

	Renderer& renderer = Renderer::Get();
	renderer.Init(SCR_WIDTH, SCR_HEIGHT);
	renderer.SetViewportMatrix(ViewPortMatrix);
	renderer.SetProjectionMatrix(ProjectionMatrix);
	renderer.LoadTexture(std::string("assets/container.jpg"));


	float angle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderer.ClearColorBuffer(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		renderer.ClearDepthBuffer();

		glm::mat4 LookAtMatrix = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		renderer.SetLookAtMatrix(LookAtMatrix);
		renderer.SetModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 1.0, 0.0)));
		renderer.DrawMesh(Box);
		renderer.Flush();

		angle += 1.0f;

		glfwSwapBuffers(window);
		glfwPollEvents();
		//cin.get();

	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		eye.z += 0.5;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		eye.z -= 0.5;
	}
}

