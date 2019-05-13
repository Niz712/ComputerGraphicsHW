#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position; //	摄像机位置
	glm::vec3 Front;	//	摄像机方向
	glm::vec3 Up;		//	摄像机上轴
	glm::vec3 Right;	//	摄像机右轴
	glm::vec3 WorldUp;	//	世界坐标的上向量
	// Euler Angles
	float Yaw;			//	偏航角
	float Pitch;		//	俯仰角
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

#endif

