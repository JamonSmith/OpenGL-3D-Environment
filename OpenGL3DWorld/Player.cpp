#include "Player.h"

extern int verticalBound;
extern int horizontalBound;

Player::Player(double x, double y, double s, double rad)
{
	posX = x;
	posY = y;
	speed = s;
	radius = rad;
}

void Player::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		posX -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		posX += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		posY += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		posY -= speed;
	}
}

void Player::checkBounds()
{
	if (posX <= -horizontalBound + radius)
	{
		posX = -horizontalBound + radius;
	}

	if (posX >= horizontalBound - radius)
	{
		posX = horizontalBound - radius;
	}

	if (posY <= -verticalBound + radius)
	{
		posY = -verticalBound + radius;
	}

	if (posY >= verticalBound - radius)
	{
		posY = verticalBound - radius;
	}
}

void Player::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(posX, posY, -30.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	int segments = 32;
	for (int i = 0; i <= 2.0 * segments; i++)
	{
		float angle = i * pi / segments;
		glVertex2f(cos(angle) * radius, sin(angle) * radius);
	}
	glEnd();
}