#include "Camera.h"
#include "Vector2.h"

Camera::Camera(double x, double y, double z, double speed, double yaw, double rotspeed)
{
	camerax = x;
	cameray = y;
	cameraz = z;
	cameraspeed = speed;
	verticalvelocity = 0.0;
	camerayaw = yaw;
	camerarotationspeed = rotspeed;
}

void Camera::movement(GLFWwindow* window, double deltaTime, const vector<AABB>& obstacles)
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

	double oldx = camerax;
	double oldz = cameraz;

	camerax += move.x * cameraspeed * deltaTime;

	for (const AABB& obstacle : obstacles)
	{
		if (getBounds().intersects(obstacle))
		{
			camerax = oldx;
			break;
		}
	}

	cameraz += move.z * cameraspeed * deltaTime;

	for (const AABB& obstacle : obstacles)
	{
		if (getBounds().intersects(obstacle))
		{
			cameraz = oldz;
			break;
		}
	}
}

void Camera::applyView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cameray = 5.0;
	
	glRotatef(camerayaw, 0.0, 1.0, 0.0);
	glTranslatef(-camerax, -cameray, -cameraz);
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

AABB Camera::getBounds() const
{
	AABB bounds
	{
		{camerax - 0.5, cameraz - 0.5},
		{camerax + 0.5, cameraz + 0.5}
	};

	return bounds;
}

void Camera::setPos(double x, double z)
{
	camerax = x;
	cameraz = z;
}

Vector2 Camera::getPos() const
{
	return {camerax, cameraz};
}

void Camera::jump(GLFWwindow* window)
{
	const double jumpheight = 10.0;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && cameray <= -5.0)
	{
		verticalvelocity = jumpheight;
	}
}

void Camera::applyGravity(double deltaTime)
{
	verticalvelocity -= 9.81 * deltaTime;
	cameray += verticalvelocity * deltaTime;

	if (cameray <= -5.0)
	{
		cameray = -5.0;
		verticalvelocity = 0.0;
	}
}