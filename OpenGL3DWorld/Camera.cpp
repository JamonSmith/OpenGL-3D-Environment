#include "Camera.h"
#include "Vector3.h"

Camera::Camera(double x, double y, double z, double speed, double yaw, double pitch, double rotspeed) :
	camerax(x), cameray(y), cameraz(z), cameraspeed(speed),
	camerayaw(yaw), camerapitch(pitch), camerarotationspeed(rotspeed),
	verticalvelocity(0.0) {}

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camerapitch -= camerarotationspeed * deltaTime;

		if (camerapitch <= -90.0)
		{
			camerapitch = -90.0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camerapitch += camerarotationspeed * deltaTime;

		if (camerapitch >= 75.0)
		{
			camerapitch = 75.0;
		}
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

	move.normalize2();

	double oldx = camerax;
	double oldz = cameraz;

	double movespeed = cameraspeed;

	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		movespeed *= 2.0;
	}

	if (crouchoffset < 0.0)
	{
		movespeed *= 0.5;
	}

	camerax += move.x * movespeed * deltaTime;

	for (const AABB& obstacle : obstacles)
	{
		if (getBounds().intersects(obstacle))
		{
			camerax = oldx;
			break;
		}
	}

	cameraz += move.z * movespeed * deltaTime;

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
	
	glRotatef(camerapitch, 1.0, 0.0, 0.0);
	glRotatef(camerayaw, 0.0, 1.0, 0.0);
	glTranslatef(-camerax, -(cameray + crouchoffset), -cameraz);
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
		Vector3(camerax - 1.0, cameray - 10.0, cameraz - 1.0),
		Vector3(camerax + 1.0, cameray + crouchoffset + 1.0, cameraz + 1.0)
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

void Camera::jump(GLFWwindow* window,const vector<AABB>& obstacles, double jumppower)
{
	const double err = 0.001;
	const double jumpheight = jumppower;
	double camerafoot = cameray - cameraheight;
	bool isGrounded = abs(camerafoot - (-15.0)) < err;

	AABB cameraBounds = getBounds();

	for (const AABB& obstacle : obstacles)
	{
		bool overlapx = cameraBounds.min.x <= obstacle.max.x && cameraBounds.max.x >= obstacle.min.x;

		bool overlapz = cameraBounds.min.z <= obstacle.max.z && cameraBounds.max.z >= obstacle.min.z;

		if (verticalvelocity == 0.0 && abs(camerafoot - obstacle.max.y) < err && overlapx && overlapz)
		{
			isGrounded = true;
			break;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && crouchoffset == 0.0 && verticalvelocity == 0.0 && isGrounded)
	{
		verticalvelocity = jumpheight;
	}
}

void Camera::crouch(GLFWwindow* window, const vector<AABB>& obstacles, double crouchheight)
{
	AABB standingBounds
	{
		Vector3(camerax - 1.0, cameray - 10.0, cameraz - 1.0),
		Vector3(camerax + 1.0, cameray + 1.0, cameraz + 1.0)
	};
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		crouchoffset = -crouchheight;
	}
	else
	{
		bool canStand = true;

		for (const AABB& obstacle : obstacles)
		{
			if (standingBounds.intersects(obstacle))
			{
				canStand = false;
				break;
			}
		}

		if (canStand)
		{
			crouchoffset = 0.0;
		}
	}	
}

void Camera::applyGravity(double deltaTime, const vector<AABB>& obstacles)
{
	double oldy = cameray;
	
	verticalvelocity -= 9.81 * deltaTime;
	cameray += verticalvelocity * deltaTime;

	if (cameray <= -5.0)
	{
		cameray = -5.0;
		verticalvelocity = 0.0;
	}

	for (const AABB& obstacle : obstacles)
	{
		AABB cameraBounds = getBounds();

		bool overlapx = cameraBounds.min.x <= obstacle.max.x && cameraBounds.max.x >= obstacle.min.x;

		bool overlapz = cameraBounds.min.z <= obstacle.max.z && cameraBounds.max.z >= obstacle.min.z;

		if (verticalvelocity < 0.0 && oldy - 10.0 >= obstacle.max.y && cameray - 10.0 <= obstacle.max.y && overlapx && overlapz)
		{
			cameray = obstacle.max.y + 10.0;
			verticalvelocity = 0.0;
			break;
		}
	}
}