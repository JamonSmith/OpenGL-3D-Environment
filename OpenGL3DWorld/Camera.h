#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#define pi 2.0 * acos(0.0)

class Camera
{
private:

	double cameraz;
	double camerax;
	double cameraspeed;

	double camerayaw;

	double camerarotationspeed;


public:

	Camera(double x, double z, double speed, double yaw, double rotspeed);

	void movement(GLFWwindow* window, double deltaTime);
	void applyView();
	void boundsCheck(double xbound, double zbound);
};