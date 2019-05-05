#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/ext/scalar_constants.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
class Camera {
public:
	Camera();
	void initialize();
	void moveForward(GLfloat const distance);
	void moveBack(GLfloat const distance);
	void moveRight(GLfloat const distance);
	void moveLeft(GLfloat const distance);
	void moveUp(GLfloat const distance);
	void moveDown(GLfloat const distance);
	void rotate(GLfloat const pitch, GLfloat const yaw);
	glm::mat4 getView();
	void turnOn();
	void turnOff();
	bool isOn();
	glm::vec3 getPos();

private:
	GLfloat pfov, pratio, pnear, pfar;
	glm::vec3 cameraTarget;
	glm::vec3 cameraPos;
	glm::vec3 cameraDirection;
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	bool on = false;
};
