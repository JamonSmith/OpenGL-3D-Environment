#include "Camera.h"
#include "Vector2.h"

Camera::Camera(double x, double z, double speed, double yaw, double rotspeed)
{
	camerax = x;
	cameraz = z;
	cameraspeed = speed;
	camerayaw = yaw;
	camerarotationspeed = rotspeed;
}

void Camera::movement(GLFWwindow* window, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camerayaw -= camerarotationspeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camerayaw += camerarotationspeed * deltaTime;
	}

	double forwardx = sin(camerayaw * (pi / 180.0));
	double forwardz = -cos(camerayaw * (pi / 180.0));

	double lateralx = sin((camerayaw + 90.0) * (pi / 180.0));
	double lateralz = -cos((camerayaw + 90.0) * (pi / 180.0));

	Vector2 move{0.0, 0.0};

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		move.x -= lateralx;
		move.z -= lateralz;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		move.x += lateralx;
		move.z += lateralz;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		move.x += forwardx;
		move.z += forwardz;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		move.x -= forwardx;
		move.z -= forwardz;
	}

	move.normalize();

	camerax += move.x * cameraspeed * deltaTime;
	cameraz += move.z * cameraspeed * deltaTime;
}

void Camera::applyView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(camerayaw, 0.0, 1.0, 0.0);
	glTranslatef(-camerax, 0.0, -cameraz);
}

void Camera::boundsCheck(double xbound, double zbound)
{
	if (camerax <= -xbound)
	{
		camerax = -xbound;
	}

	if (camerax >= xbound)
	{
		camerax = xbound;
	}

	if (cameraz <= -zbound)
	{
		cameraz = -zbound;
	}

	if (cameraz >= zbound)
	{
		cameraz = zbound;
	}
}