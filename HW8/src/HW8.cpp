#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include <learnopengl/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);

void addVertices(glm::vec2 point);
void renderPointsAndLines(int size = 5, bool ifLine = false, bool ifClear = true);
float Bernstein(int i, int n, float t);
int Factorial(int x);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Bezier
int controlPoints_num;
glm::vec2 controlPoints[101];
glm::vec2 tempPoints[300];
glm::vec2 middlePoints[300];
float vertices[4002];
int vertices_num = 0;
int fac = 1;
long long int facArray[105];
float frame;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader shader("HW8.vs", "HW8.fs");
	shader.use();

	facArray[0] = 1;

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw points
		for (int i = 0; i < controlPoints_num; i++)
		{
			addVertices(controlPoints[i]);
		}
		renderPointsAndLines(5, true, false);
		//renderPointsAndLines();

		// draw curves
		if (controlPoints_num > 1) {
			for (float t = 0; t < 1; t += 0.001) {
				glm::vec2 p = controlPoints[0] * Bernstein(0, controlPoints_num - 1, t);
				for (int i = 1; i < controlPoints_num; i++) {
					p = p + controlPoints[i] * Bernstein(i, controlPoints_num - 1, t);
				}
				addVertices(p);
			}
			renderPointsAndLines(1);
		}

		// draw lines
		if (controlPoints_num > 1) {
			frame += 0.001;
			if (frame > 1) {
				frame = 0;
			}
			float t = frame;
			int mCount = controlPoints_num;
			for (int i = 0; i < controlPoints_num; i++) {
				middlePoints[i] = controlPoints[i];
			}
			while (mCount > 1) {
				int nCount = 0;
				for (int i = 0; i < mCount - 1; i++) {
					glm::vec2 p = middlePoints[i] * (1 - t) + middlePoints[i + 1] * t;
					tempPoints[nCount] = p;
					nCount++;
					addVertices(p);
				}
				renderPointsAndLines(5, true);
				for (int i = 0; i < nCount; i++) {
					middlePoints[i] = tempPoints[i];
				}
				mCount = nCount;
			}
			glm::vec2 p = controlPoints[0] * Bernstein(0, controlPoints_num - 1, t);
			for (int i = 1; i < controlPoints_num; i++) {
				p = p + controlPoints[i] * Bernstein(i, controlPoints_num - 1, t);
			}
			addVertices(p);
			renderPointsAndLines();
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//[in]	window	The window that received the event.
//[in]	button	The mouse button that was pressed or released.
//[in]	action	One of GLFW_PRESS or GLFW_RELEASE.
//[in]	mods	Bit field describing which modifier keys were held down.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (controlPoints_num < 21) {
			controlPoints[controlPoints_num] = glm::vec2(xpos, ypos);
			controlPoints_num++;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (controlPoints_num > 0) {
			controlPoints_num--;
		}
	}
}

void addVertices(glm::vec2 point) {
	vertices[vertices_num * 2] = (point.x / SCR_WIDTH) * 2 - 1;
	vertices[vertices_num * 2 + 1] = -((point.y / SCR_HEIGHT) * 2 - 1);
	vertices_num++;
}

void renderPointsAndLines(int size, bool ifLine, bool ifClear) {
	if (vertices_num <= 0)
	{
		return;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glPointSize(size);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * vertices_num, vertices, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_POINTS, 0, vertices_num);

	if (ifLine)
	{
		glDrawArrays(GL_LINE_STRIP, 0, vertices_num);
	}
	glBindVertexArray(0);

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (ifClear)
	{
		vertices_num = 0;
	}
}

float Bernstein(int i, int n, float t) {
	long long int a = Factorial(i) * Factorial(n - i);
	float res = Factorial(n) / a;
	res *= pow(t, i) * pow(1 - t, n - i);
	return res;
}

int Factorial(int x) {
	if (x > 104) {
		return 1;
	}
	for (; fac <= x; fac++) {
		facArray[fac] = facArray[fac - 1] * fac;
	}
	return facArray[x];
}