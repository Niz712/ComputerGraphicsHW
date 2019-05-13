#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>
using namespace std;

#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include "Camera.h"

// 窗口回调函数，在每次窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 0 - orthographic projection
// 1 - perspective projection
// 2 - View Changing
// 3 - camera
int choice = 0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// 初始化glfw窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 下面这条适配Mac OsX系统
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	// 注册窗口回调函数，在窗口调整大小时调用回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("HW5.vs", "HW5.fs");

	float vertices[] = {
		// positons				// colors
	   -2.0f, -2.0f, -2.0f,		1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f,		1.0f, 0.0f, 0.0f,
		2.0f,  2.0f, -2.0f,		1.0f, 0.0f, 0.0f,
		2.0f,  2.0f, -2.0f,		1.0f, 0.0f, 0.0f,
	   -2.0f,  2.0f, -2.0f,		1.0f, 0.0f, 0.0f,
	   -2.0f, -2.0f, -2.0f,		1.0f, 0.0f, 0.0f,

	   -2.0f, -2.0f,  2.0f,		1.0f, 1.0f, 0.0f,
		2.0f, -2.0f,  2.0f,		1.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  2.0f,		1.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  2.0f,		1.0f, 1.0f, 0.0f,
	   -2.0f,  2.0f,  2.0f,		1.0f, 1.0f, 0.0f,
	   -2.0f, -2.0f,  2.0f,		1.0f, 1.0f, 0.0f,

	   -2.0f,  2.0f,  2.0f,		1.0f, 0.0f, 1.0f,
	   -2.0f,  2.0f, -2.0f,		1.0f, 0.0f, 1.0f,
	   -2.0f, -2.0f, -2.0f,		1.0f, 0.0f, 1.0f,
	   -2.0f, -2.0f, -2.0f,		1.0f, 0.0f, 1.0f,
	   -2.0f, -2.0f,  2.0f,		1.0f, 0.0f, 1.0f,
	   -2.0f,  2.0f,  2.0f,		1.0f, 0.0f, 1.0f,

		2.0f,  2.0f,  2.0f,		0.0f, 1.0f, 0.0f,
		2.0f,  2.0f, -2.0f,		0.0f, 1.0f, 0.0f,
		2.0f, -2.0f, -2.0f,		0.0f, 1.0f, 0.0f,
		2.0f, -2.0f, -2.0f,		0.0f, 1.0f, 0.0f,
		2.0f, -2.0f,  2.0f,		0.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  2.0f,		0.0f, 1.0f, 0.0f,

	   -2.0f, -2.0f, -2.0f,		0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,		0.0f, 1.0f, 1.0f,
		2.0f, -2.0f,  2.0f,		0.0f, 1.0f, 1.0f,
		2.0f, -2.0f,  2.0f,		0.0f, 1.0f, 1.0f,
	   -2.0f, -2.0f,  2.0f,		0.0f, 1.0f, 1.0f,
	   -2.0f, -2.0f, -2.0f,		0.0f, 1.0f, 1.0f,

	   -2.0f,  2.0f, -2.0f,		0.0f, 0.0f, 1.0f,
		2.0f,  2.0f, -2.0f,		0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,		0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,		0.0f, 0.0f, 1.0f,
	   -2.0f,  2.0f,  2.0f,		0.0f, 0.0f, 1.0f,
	   -2.0f,  2.0f, -2.0f,		0.0f, 0.0f, 1.0f,
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
	// 创建并绑定ImGui
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	const char* glsl_version = "#version 130";
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// settings
	float ortho_left = -10.0f;
	float ortho_right = 10.0f;
	float ortho_bottom = -10.0f;
	float ortho_top = 10.0f;
	float ortho_near = 0.1f;
	float ortho_far = 50.0f;

	float fov_radius = 30.0f;
	float aspect_radio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	float persp_near = 0.1f;
	float persp_far = 50.0f;

	//ourShader.setInt("count", 0);
	ourShader.use();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Camera");
		ImGui::Text("Choose: ");

		ImGui::RadioButton("orthographic projection", &choice, 0);
		ImGui::SliderFloat("left", &ortho_left, -50.0f, 50.f);
		ImGui::SliderFloat("right", &ortho_right, -50.0f, 50.f);
		ImGui::SliderFloat("top", &ortho_top, -50.0f, 50.f);
		ImGui::SliderFloat("bottom", &ortho_bottom, -50.0f, 50.f);
		ImGui::SliderFloat("near", &ortho_near, 0.1f, 50.0f);
		ImGui::SliderFloat("far", &ortho_far, 0.1f, 50.0f);

		ImGui::RadioButton("perspective projection", &choice, 1);
		ImGui::SliderFloat("Field of View", &fov_radius, -180.0f, 180.0f);
		ImGui::SliderFloat("Aspect Radio", &aspect_radio, 0.1f, 2.0f);
		ImGui::SliderFloat("persp near", &persp_near, 0.1f, 50.0f);
		ImGui::SliderFloat("persp far", &persp_far, 0.1f, 50.0f);

		ImGui::RadioButton("View Changing", &choice, 2);
		ImGui::RadioButton("Enable Camera", &choice, 3);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// 把上次作业绘制的cube放置在(-1.5, 0.5, -1.5)位置，要求6个面颜色不一致
		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -1.5f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -30.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		if (choice == 0) {
			projection = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);
		}
		else if (choice == 1) {
			projection = glm::perspective(glm::radians(fov_radius), aspect_radio, persp_near, persp_far);
		}
		else if (choice == 2) {
			// 把cube放置在(0, 0, 0)处
			model = glm::mat4(1.0f);
			// 使摄像机围绕cube旋转，并且时刻看着cube中心
			float radius = 30.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}
		else if (choice == 3) {
			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}

		// 旋转一定角度
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.1f, 0.2f, 0.3f));

		// 简化版本
		ourShader.use();
		ourShader.setMat4("model", model);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render container
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//渲染指令
		int view_width, view_height;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		//双缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		choice = 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}