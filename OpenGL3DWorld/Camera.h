#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

#include "AABB.h"

#define pi 2.0 * acos(0.0)

using namespace std;

class Camera
{
private:

	double cameraz;
	double camerax;
	double cameraspeed;

	double cameray;
	double verticalvelocity;

	double camerayaw;
	double camerapitch;

	double camerarotationspeed;

	double crouchoffset = 0.0;
	double cameraheight = 10.0 - crouchoffset;

public:

	Camera(double x, double y, double z, double speed, double yaw, double pitch, double rotspeed);

	void movement(GLFWwindow* window, double deltaTime, const vector<AABB>& obstacles);
	void applyView();
	void boundsCheck(double xbound, double zbound);
	AABB getBounds() const;

	void setPos(double x, double z);
	Vector2 getPos() const;

	void jump(GLFWwindow* window, const vector<AABB>& obstacles, double jumppower);
	void crouch(GLFWwindow* window, const vector<AABB>& obstacles, double crouchheight);
	void applyGravity(double deltaTime, const vector<AABB>& obstacles);
};