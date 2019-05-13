#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
using namespace std;

// 窗口回调函数，在每次窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);

//
vector<int> Bresenham(int x0, int y0, int x1, int y1);
vector<int> DrawTriangleBresenham(int x1, int y1, int x2, int y2, int x3, int y3);

void getAllPoints(int x0, int y0, int x, int y, vector<int> &points);
vector<int> DrawCircleBresenham(int x0, int y0, int r);

float *creatVertices(vector<int> v);

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

	bool draw_circle = false;
	int point1[2] = { 0, 200 };
	int point2[2] = { -100, 0 };
	int point3[2] = { 100, 0 };
	float radius = 100.0f;

	//渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		processInput(window);


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (draw_circle) {
			ImGui::Begin("Edit circle");
			ImGui::Text("Input radius: ");
			ImGui::SliderFloat("R", &radius, 0.0f, 100.0f);
		}
		else
		{
			ImGui::Begin("Edit points");
			ImGui::Text("Input 3 points: ");
			ImGui::SliderInt2("point1", point1, -180, 180);
			ImGui::SliderInt2("point2", point2, -180, 180);
			ImGui::SliderInt2("point3", point3, -180, 180);
		}
		ImGui::Checkbox("DRAW CIRCLE", &draw_circle);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		vector<int> test;

		if (draw_circle) {
			test = DrawCircleBresenham(0, 0, (int)radius);
		}
		else
		{
			test = DrawTriangleBresenham(point1[0], point1[1], point2[0], point2[1], point3[0], point3[1]);
		}
		int point_num = test.size() / 2;
		int total = point_num * 3;
		float *vertices = creatVertices(test);

		// 渲染
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象：

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
		glBufferData(GL_ARRAY_BUFFER, total * sizeof(float), vertices, GL_STREAM_DRAW);//把之前定义的顶点数据复制到缓冲的内存中

																				  //解析定点数据:位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);


		glUseProgram(shaderProgram);//激活程序对象

		delete vertices;


		//渲染指令
		ImGui::Render();
		int view_width, view_height;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glDrawArrays(GL_POINTS, 0, point_num);


		glfwMakeContextCurrent(window);
		//双缓冲
		glfwSwapBuffers(window);

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

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

vector<int> Bresenham(int x0, int y0, int x1, int y1) {
	vector<int> points;
	points.push_back(x0);
	points.push_back(y0);
	int dx = x1 - x0;
	int dy = y1 - y0;
	int direct_x = dx > 0 ? 1 : -1;
	int direct_y = dy > 0 ? 1 : -1;
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	if (abs(dx) > abs(dy)) {
		int p = 2 * dy - dx;
		int x = x0;
		int y = y0;
		int dy2 = 2 * dy;
		int dy2_sub_dx2 = 2 * dy - 2 * dx;

		while (x != x1) {
			points.push_back(x);
			points.push_back(y);
			if (p > 0) {
				y += direct_y;
				p += dy2_sub_dx2;
			}
			else
			{
				p += dy2;
			}
			x += direct_x;
		}
	}
	else {
		int p = 2 * dx - dy;
		int x = x0;
		int y = y0;
		int dx2 = 2 * dx;
		int dx2_sub_dy2 = 2 * dx - 2 * dy;
		while (y != y1) {
			points.push_back(x);
			points.push_back(y);
			if (p > 0) {
				x += direct_x;
				p += dx2_sub_dy2;
			}
			else
			{
				p += dx2;
			}
			y += direct_y;
		}
	}
	points.push_back(x1);
	points.push_back(y1);
	return points;
}

vector<int> DrawTriangleBresenham(int x1, int y1, int x2, int y2, int x3, int y3) {
	vector<int> points;
	vector< vector<int> > edges;
	edges.push_back(Bresenham(x1, y1, x2, y2));
	edges.push_back(Bresenham(x1, y1, x3, y3));
	edges.push_back(Bresenham(x3, y3, x2, y2));
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++) {
			points.push_back(edges[i][j]);
		}
	}
	return points;
}

void getAllPoints(int x0, int y0, int x, int y, vector<int> &points) {
	points.push_back(x0 + x); points.push_back(y0 + y);
	points.push_back(x0 + x); points.push_back(y0 - y);
	points.push_back(x0 - x); points.push_back(y0 + y);
	points.push_back(x0 - x); points.push_back(y0 - y);
	points.push_back(x0 + y); points.push_back(y0 + x);
	points.push_back(x0 + y); points.push_back(y0 - x);
	points.push_back(x0 - y); points.push_back(y0 + x);
	points.push_back(x0 - y); points.push_back(y0 - x);
}

vector<int> DrawCircleBresenham(int x0, int y0, int r) {
	vector<int> points;
	int x, y, d;
	y = r;
	x = 0;
	d = 3 - 2 * r;
	getAllPoints(x0, y0, x, y, points);
	while (x < y) {
		if (d < 0) {
			d += 4 * x + 6;
		}
		else
		{
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;
		getAllPoints(x0, y0, x, y, points);
	}
	return points;
}

float *creatVertices(vector<int> v) {
	int point_num = v.size() / 2;
	int total = point_num * 3;
	float *vertices = new float[total];
	for (int i = 0; i < point_num; i++) {
		vertices[i * 3 + 0] = (float)v[i * 2 + 0] / (float)640;
		vertices[i * 3 + 1] = (float)v[i * 2 + 1] / (float)360;
		vertices[i * 3 + 2] = 0.0f;
	}
	return vertices;
}