#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/ext/scalar_constants.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "Camera.h"
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);



Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0, lastY = 0;
float pitch = 0, yaw = 270.0f;
bool isOn = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	//初始化glfw
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = NULL;
	window = glfwCreateWindow(800, 600, "CG_HW7", NULL, NULL);
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

	//绑定回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glViewport(0, 0, 800, 600);

	//打开深度测试
	glEnable(GL_DEPTH_TEST);

	//加载着色器
	Shader phongLightingShader("lightphong.vs", "lightphong.fs");
	Shader gouraudLightingShader("lightgouraud.vs", "lightgouraud.fs");
	Shader lampShader("lamp.vs", "lamp.fs");
	Shader simpleDepthShader("simpleDepthShader.vs", "simpleDepthShader.fs");

	//顶点数据
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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

	//生成VBO以及VAO并绑定
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//生成lightVAO并绑定
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//平面顶点数据
	float vertices_plane[] = {
		-100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f,
		-100.0f, -0.5f,  100.0f, 0.0f, 1.0f, 0.0f,
		 100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f,
		 100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f,
		-100.0f, -0.5f,  100.0f, 0.0f, 1.0f, 0.0f,
		 100.0f, -0.5f,  100.0f, 0.0f, 1.0f, 0.0f
	};

	//深度贴图
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	GLuint shadow_width = 1024, shadow_height = 1024;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	phongLightingShader.use();
	phongLightingShader.setInt("diffuseTexture", 0);
	phongLightingShader.setInt("shadowMap", 0);
	gouraudLightingShader.use();
	gouraudLightingShader.setInt("diffuseTexture", 0);
	gouraudLightingShader.setInt("shadowMap", 0);

	//加载Imgui
	const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//投影参数
	bool perspective_on = true;
	float perspective_angle = 45.0f;
	int src_width = 800;
	int src_height = 600;
	float neardis = 0.1f;
	float fardis = 20.0f;
	bool orthographic_on = false;
	float left = -2.0f;
	float right = 2.0f;
	float bottom = -2.0f;
	float top = 2.0f;
	int camera_mode = 0;
	int light_mode = 0;

	//光照参数
	float ambientStrength = 0.1;
	float diffuseStrength = 1;
	float specularStrength = 0.5;
	float reflectExponent = 2;
	float lightColor[3] = { 1.0f, 1.0f, 1.0f };
	float objectColor[3] = { 0.5f, 0.5f, 0.5f };
	float lightPosition[3] = { 1.2f, 1.0f, 2.0f };
	float planeColor[3] = { 0.5f, 0.7f, 0.65f };

	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui菜单栏
		{
			if (ImGui::BeginMainMenuBar()) {
				//选择投影的模式
				if (ImGui::BeginMenu("Projection Mode")) {
					if (ImGui::MenuItem("Perspective")) {
						perspective_on = true;
						orthographic_on = false;
					}
					if (ImGui::MenuItem("Orthographic")) {
						perspective_on = false;
						orthographic_on = true;
					}
					ImGui::EndMenu();
				}
				//选择观察的模式
				if (ImGui::BeginMenu("Camera Mode")) {
					if (ImGui::MenuItem("Static")) {
						camera_mode = 0;
					}
					if (ImGui::MenuItem("Auto Rotate")) {
						camera_mode = 1;
					}
					if (ImGui::MenuItem("Manual")) {
						camera_mode = 2;
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						camera.turnOn();
						isOn = false;
					}
					ImGui::EndMenu();
				}
				//选择着色的模式
				if (ImGui::BeginMenu("Shading Mode")) {
					if (ImGui::MenuItem("Phong")) {
						light_mode = 0;
					}
					if (ImGui::MenuItem("Gauroud")) {
						light_mode = 1;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		//声明model, view, 投影矩阵
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 perspective = glm::mat4(1.0f);
		glm::mat4 orthographic = glm::mat4(1.0f);

		//生成各个模式下的view矩阵
		if (camera_mode == 0) {
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
		}
		else if (camera_mode == 1) {
			float radius = 10.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}
		else if (camera_mode == 2) {
			view = camera.getView();
		}

		//ImGui调整投影参数
		if (perspective_on) {
			ImGui::Begin("Perspective Setting");
			ImGui::SliderFloat("Angle", &perspective_angle, 1.0f, 45.0f);
			ImGui::SliderInt("Width", &src_width, 1, 1024);
			ImGui::SliderInt("Height", &src_height, 1, 768);
			ImGui::SliderFloat("zNear", &neardis, 0.1f, 20.0f);
			ImGui::SliderFloat("zFar", &fardis, 0.1f, 20.0);
			perspective = glm::perspective(glm::radians(perspective_angle), (float)src_width / (float)src_height, neardis, fardis);
			ImGui::End();
		}
		else if (orthographic_on) {
			ImGui::Begin("Orthographic Setting");
			ImGui::SliderFloat("Left", &left, -5.0f, 5.0f);
			ImGui::SliderFloat("Right", &right, -5.0f, 5.0f);
			ImGui::SliderFloat("Bottom", &bottom, -5.0f, 5.0f);
			ImGui::SliderFloat("Top", &top, -5.0f, 5.0f);
			ImGui::SliderFloat("Near", &neardis, 0.1f, 20.0f);
			ImGui::SliderFloat("Far", &fardis, 0.1f, 20.0f);
			orthographic = glm::ortho(left, right, bottom, top, neardis, fardis);
			ImGui::End();
		}

		//ImGui调整光照参数
		ImGui::Begin("Light Setting");
		ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("reflectExponent", &reflectExponent, 1.0f, 512.0f);
		ImGui::ColorEdit3("light color", lightColor);
		ImGui::ColorEdit3("plane color", planeColor);
		ImGui::ColorEdit3("object color", objectColor);
		ImGui::SliderFloat3("lightPos", lightPosition, -3.0f, 3.0f);
		ImGui::End();

		//将lightPosition数组转化为vec3类型的变量
		lightPos.x = lightPosition[0];
		lightPos.y = lightPosition[1];
		lightPos.z = lightPosition[2];


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		//渲染深度贴图
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		glViewport(0, 0, shadow_width, shadow_height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//使用simpleDepthShader场景渲染
		simpleDepthShader.setMat4("model", model);
		//平面
		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_plane), vertices_plane, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//方块
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//重置viewport
		glViewport(0, 0, src_width, src_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//使用phongLightingShader或是gouraudLightingShader渲染场景
		//参数调整
		if (light_mode == 0) {
			phongLightingShader.use();
			phongLightingShader.setFloat("ambientStrength", ambientStrength);
			phongLightingShader.setFloat("diffuseStrength", diffuseStrength);
			phongLightingShader.setFloat("specularStrength", specularStrength);
			phongLightingShader.setFloat("reflectExponent", reflectExponent);
			phongLightingShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			phongLightingShader.setVec3("lightPos", lightPosition[0], lightPosition[1], lightPosition[2]);
			phongLightingShader.setVec3("viewPos", camera.getPos());
			phongLightingShader.setMat4("model", model);
			phongLightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			if (perspective_on) {
				phongLightingShader.setMat4("projection", perspective);
			}
			else if (orthographic_on) {
				phongLightingShader.setMat4("projection", orthographic);
			}
			phongLightingShader.setMat4("view", view);
		}
		else if (light_mode == 1) {
			gouraudLightingShader.use();
			gouraudLightingShader.setFloat("ambientStrength", ambientStrength);
			gouraudLightingShader.setFloat("diffuseStrength", diffuseStrength);
			gouraudLightingShader.setFloat("specularStrength", specularStrength);
			gouraudLightingShader.setFloat("reflectExponent", reflectExponent);
			gouraudLightingShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			gouraudLightingShader.setVec3("lightPos", lightPos);
			gouraudLightingShader.setVec3("viewPos", camera.getPos());
			gouraudLightingShader.setMat4("model", model);
			gouraudLightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			if (perspective_on) {
				gouraudLightingShader.setMat4("projection", perspective);
			}
			else if (orthographic_on) {
				gouraudLightingShader.setMat4("projection", orthographic);
			}
			gouraudLightingShader.setMat4("view", view);
		}
		//平面
		if (light_mode == 0) {
			phongLightingShader.setVec3("objectColor", planeColor[0], planeColor[1], planeColor[2]);
		}
		else if (light_mode == 1) {
			gouraudLightingShader.setVec3("objectColor", planeColor[0], planeColor[1], planeColor[2]);
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_plane), vertices_plane, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//方块
		if (light_mode == 0) {
			phongLightingShader.setVec3("objectColor", objectColor[0], objectColor[1], objectColor[2]);
		}
		else if (light_mode == 1) {
			gouraudLightingShader.setVec3("objectColor", objectColor[0], objectColor[1], objectColor[2]);
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//渲染灯光
		lampShader.use();
		lampShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
		lampShader.setMat4("view", view);
		if (perspective_on) {
			lampShader.setMat4("projection", perspective);
		}
		else if (orthographic_on) {
			lampShader.setMat4("projection", orthographic);
		}
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ImGui渲染
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
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (!camera.isOn()) return;
	float cameraSpeed = 3.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.moveUp(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.moveDown(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		camera.turnOff();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!camera.isOn()) return;
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	if (!isOn) {
		lastX = xpos;
		lastY = ypos;
		isOn = true;
		return;
	}
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch -= yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	if (yaw > 360.0f)
		yaw -= 360.0f;
	if (yaw < -360.0f)
		yaw += 360.0f;

	camera.rotate(pitch, yaw);
}