#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>
using namespace std;

// 窗口回调函数，在每次窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("HW4.vs", "HW4.fs");

	float vertices[] = {
		// positions			// colors
		-2.0f, -2.0f, -2.0f,	1.0f, 0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,	0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,	0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,	1.0f, 0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,	0.0f, 1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,	0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,	1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,	0.0f, 1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3,

		0, 1, 4,
		1, 4, 5,

		0, 2, 4,
		2, 4, 6,

		1, 3, 5,
		3, 5, 7,

		2, 3, 7,
		2, 6, 7,

		4, 5, 6,
		5, 6, 7
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


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

	// transformation setting
	bool depth_test = false;
	bool horizotal_translate = false;
	float h_ft = 0.0f;
	bool rh = true;
	bool vertical_translate = false;
	float v_ft = 0.0f;
	bool rv = true;
	bool rotation = false;
	bool scaling = false;
	float fs = 1.0f;
	bool rs = true;

	ourShader.use();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Transformation");
		ImGui::Text("Choose transformation: ");

		ImGui::Checkbox("Depth Test", &depth_test);
		ImGui::Checkbox("Horizotal Transformation", &horizotal_translate);
		ImGui::Checkbox("Vertical Transformation", &vertical_translate);
		ImGui::Checkbox("Rotation", &rotation);
		ImGui::Checkbox("Scaling", &scaling);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (depth_test) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		//view & projection
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -40.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		if (horizotal_translate) {
			if (h_ft < 12.0f && rh) {
				h_ft += 0.1f;
				model = glm::translate(model, glm::vec3(h_ft, 0.0f, 0.0f));
			}
			else {
				rh = h_ft < -12.0f;
				h_ft -= 0.1f;
				model = glm::translate(model, glm::vec3(h_ft, 0.0f, 0.0f));
			}
		}
		else {
			h_ft = 0.0f;
			rh = true;
		}

		if (vertical_translate) {
			if (v_ft < 12.0f && rv) {
				v_ft += 0.1f;
				model = glm::translate(model, glm::vec3(0.0f, v_ft, 0.0f));
			}
			else {
				rv = v_ft < -12.0f;
				v_ft -= 0.1f;
				model = glm::translate(model, glm::vec3(0.0f, v_ft, 0.0f));
			}
		}
		else {
			v_ft = 0.0f;
			rv = true;
		}

		if (rotation) {
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));
		}

		if (scaling) {
			if (fs < 3.0f && rs) {
				fs += 0.01f;
				model = glm::scale(model, glm::vec3(fs, fs, fs));
			}
			else {
				rs = fs < 0.1f;
				fs -= 0.01f;
				model = glm::scale(model, glm::vec3(fs, fs, fs));
			}
		}
		else {
			fs = 1.0f;
			rs = true;
		}

		// get matrix's uniform location and set matrix
		//获取着色器程序uniform
		/*unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);*/

		// 简化版本
		ourShader.setMat4("model", model);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		ourShader.use();


		//渲染指令
		ImGui::Render();
		int view_width, view_height;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// render container
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		glfwMakeContextCurrent(window);
		//双缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}