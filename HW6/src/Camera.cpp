#include "Camera.h"



Camera::Camera()
{
	on = false;
	cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cameraFront = -cameraDirection;
	glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
}

void Camera::initialize()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cameraFront = -cameraDirection;
	glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
}

void Camera::moveForward(GLfloat const distance)
{
	cameraPos += cameraFront * distance;
}

void Camera::moveBack(GLfloat const distance)
{
	cameraPos -= cameraFront * distance;
}

void Camera::moveRight(GLfloat const distance)
{
	cameraPos += cameraRight * distance;
}

void Camera::moveLeft(GLfloat const distance)
{
	cameraPos -= cameraRight * distance;
}

void Camera::moveUp(GLfloat const distance)
{
	cameraPos += cameraUp * distance;
}

void Camera::moveDown(GLfloat const distance)
{
	cameraPos -= cameraUp * distance;
}

void Camera::rotate(GLfloat const pitch, GLfloat const yaw)
{
	cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(cameraFront);

	cameraDirection = -cameraFront;
	glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::turnOn()
{
	on = true;
}

void Camera::turnOff()
{
	on = false;
}

bool Camera::isOn()
{
	return on;
}

glm::vec3 Camera::getPos() {
	return cameraPos;
}