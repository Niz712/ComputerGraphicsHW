#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

// 窗口回调函数，在每次窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"			// 位置变量的属性位置值为 0 
"layout (location = 1) in vec3 aColor;\n"		// 颜色变量的属性位置值为 1
"out vec3 ourColor;\n"				// 向片段着色器输出一个颜色
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"			// 将ourColor设置为我们从顶点数据那里得到的输入颜色
"}\0";

// 片段着色器
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	// 顶点输入
	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	// 顶点缓冲对象(Vertex Buffer Objects, VBO)
	// 顶点数组对象 VAO
	unsigned int VBO, VAO;

	// 创建一个着色器对象
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 把这个着色器源码附加到着色器对象上，然后编译它
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// 检测着色器编译成功与否
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 片段着色器对象
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 检查编译是否成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// 把着色器附加到程序对象上并链接
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检查链接是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 激活着色器程序对象
	// glUseProgram(shaderProgram);

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

	std::vector<ImVec4> colors;
	ImVec4 color_whole = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.00f));
	colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.00f));
	colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.00f));
	bool if_change_whole = false;

	//渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Color");
			ImGui::Text("This is some useful text.");			// Display some text (you can use a format strings too)
			ImGui::ColorEdit3("RIGHT", (float*)&colors[0]);		// Edit 3 floats representing a color
			ImGui::ColorEdit3("LEFT", (float*)&colors[1]);		// Edit 3 floats representing a color
			ImGui::ColorEdit3("TOP", (float*)&colors[2]);		// Edit 3 floats representing a color

			ImGui::Checkbox("IF CHANGE THE WHOLE COLOR", &if_change_whole);
			ImGui::ColorEdit3("WHOLE COLOR", (float*)&color_whole, 1);
			//Change the color
			if (if_change_whole) {
				for (int i = 0; i < 3; i++) {
					vertices[i * 6 + 3] = color_whole.x;
					vertices[i * 6 + 4] = color_whole.y;
					vertices[i * 6 + 5] = color_whole.z;
				}
			}
			else {
				for (int i = 0; i < 3; i++) {
					vertices[i * 6 + 3] = colors[i].x;
					vertices[i * 6 + 4] = colors[i].y;
					vertices[i * 6 + 5] = colors[i].z;
				}
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 渲染
		// 清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 记得激活着色器
		glUseProgram(shaderProgram);

		// 绑定VAO&VBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// 绑定顶点数组对象
		glBindVertexArray(VAO);
		// 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
		// 我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 把之前定义的顶点数据复制到缓冲的内存中
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

		// 解析顶点数据
		// 位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// 颜色属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// 绘制三角形
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		//glfwPollEvents();

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}