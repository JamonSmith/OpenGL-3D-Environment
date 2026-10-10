#pragma once

#include <GLFW/glfw3.h>
#include <cmath>

#define pi 2.0 * acos(0.0)

class Player
{
private:

	double posX;
	double posY;
	double speed;
	double radius;

public:

	Player(double x, double y, double s, double rad);

	void processInput(GLFWwindow* window);
	void checkBounds();
	void draw();
};