
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "BezierCurve.h"

using namespace std;

int w = 1, h = 1;
vector<point> cpoint;
float xpos = 0, ypos = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void cursor_position_callback(GLFWwindow* window, double x, double y) {
	xpos = (float)(x - w / 2) / w * 2;
	ypos = -(float)(y - h / 2) / h * 2;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			cpoint.push_back(point(xpos, ypos));
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (!cpoint.empty()) {
				cpoint.pop_back();
			}
		}
	}
}

int main()
{
	//初始化glfw
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = NULL;
	window = glfwCreateWindow(900, 900, "CG_HW3", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, 900, 900);
	w = 900;
	h = 900;

	//生成VAO和VBO并绑定
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//设置顶点属性指针
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//创建着色程序
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//加载Imgui
	const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	BezierCurve bcurve;
	

	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if (cpoint.size() > 0) {
			bcurve.drawBezier(cpoint, cpoint.size() * 10);
			float *vertices = new float[cpoint.size() * 2];
			for (int i = 0; i < cpoint.size(); i++) {
				vertices[i * 2] = cpoint[i].x;
				vertices[i * 2 + 1] = cpoint[i].y;
			}

			glPointSize(5.0f);
			glUseProgram(shaderProgram);
			glBufferData(GL_ARRAY_BUFFER, cpoint.size() * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, cpoint.size());
			glDrawArrays(GL_LINE_STRIP, 0, cpoint.size());
			delete[]vertices;

			vector<point> bpoint = bcurve.getBpoint();
			vertices = new float[bpoint.size() * 2];
			for (int i = 0; i < bpoint.size(); i++) {
				vertices[i * 2] = bpoint[i].x;
				vertices[i * 2 + 1] = bpoint[i].y;
			}
			glBufferData(GL_ARRAY_BUFFER, bpoint.size() * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
			glDrawArrays(GL_LINE_STRIP, 0, bpoint.size());
			delete[]vertices;

			float t = (sin((float)glfwGetTime()) + 1) / 2;
			vector<point> tpoint = cpoint;
			for (int i = 0; i < cpoint.size() - 1; i++) {
				vector<point> npoint;
				for (int j = 0; j < tpoint.size() - 1; j++) {
					point p;
					p.x = (1 - t) * tpoint[j].x + t * tpoint[j + 1].x;
					p.y = (1 - t) * tpoint[j].y + t * tpoint[j + 1].y;
					npoint.push_back(p);
				}
				vertices = new float[npoint.size() * 2];
				for (int i = 0; i < npoint.size(); i++) {
					vertices[i * 2] = npoint[i].x;
					vertices[i * 2 + 1] = npoint[i].y;
				}
				glBufferData(GL_ARRAY_BUFFER, npoint.size() * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, npoint.size());
				glDrawArrays(GL_LINE_STRIP, 0, npoint.size());
				delete[]vertices;
				tpoint = npoint;
			}
		}

		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);

		glfwSwapBuffers(window);

	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	w = width;
	h = height;
}