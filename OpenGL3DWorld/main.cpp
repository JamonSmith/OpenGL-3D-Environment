#include <iostream>
#include <vector>

#include "AABB.h"

#include "Camera.h"
#include "Player.h"

#define pi 2.0 * acos(0.0)

int screenWidth = 1000;
int screenHeight = 800;

int horizontalBound = 100;
int verticalBound = 80;

using namespace std;

void buffer_size_callback(GLFWwindow* win, int width, int height)
{
    if (height <= 0)
    {
		height = 1;
    }

	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //glOrtho(-horizontalBound, horizontalBound, -verticalBound, verticalBound, -100, 100);
    glFrustum(-1.25, 1.25, -1.0, 1.0, 1.0, 100.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void closeWindow(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void drawSquare(double x, double y, double sidelen)
{
    double halfside = sidelen / 2.0;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    glVertex2f(x - halfside, y - halfside);
    glVertex2f(x + halfside, y - halfside);
    glVertex2f(x + halfside, y + halfside);
    glVertex2f(x - halfside, y + halfside);
    glEnd();
}

void drawCube(double centerx, double centery, double centerz, double sidelen, double ang)
{
    double halfside = sidelen / 2.0;

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glRotatef(ang, 0.0, 1.0, 0.0);
    glRotatef(ang, 1.0, 0.0, 0.0);

    // Bottom side
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx - halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery - halfside, centerz - halfside);
    glVertex3f(centerx - halfside, centery - halfside, centerz - halfside);
    glEnd();

    // Right side
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx + halfside, centery - halfside, centerz - halfside);
    glVertex3f(centerx + halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz - halfside);
    glEnd();

    // Back side
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx - halfside, centery - halfside, centerz - halfside);
    glVertex3f(centerx + halfside, centery - halfside, centerz - halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz - halfside);
    glVertex3f(centerx - halfside, centery + halfside, centerz - halfside);
    glEnd();

    // Left side
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx - halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx - halfside, centery - halfside, centerz - halfside);
    glVertex3f(centerx - halfside, centery + halfside, centerz - halfside);
    glVertex3f(centerx - halfside, centery + halfside, centerz + halfside);
    glEnd();

    // Top side
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx - halfside, centery + halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz - halfside);
    glVertex3f(centerx - halfside, centery + halfside, centerz - halfside);
    glEnd();

    // Front side
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(centerx - halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery - halfside, centerz + halfside);
    glVertex3f(centerx + halfside, centery + halfside, centerz + halfside);
    glVertex3f(centerx - halfside, centery + halfside, centerz + halfside);
    glEnd();

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version (4.6 core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Test", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, buffer_size_callback);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    buffer_size_callback(window, width, height);

	double mapWidth = 100.0;

	Camera camera(0.0, 20.0, 20.0, 20.0, 0.0, 0.0, 90.0);

	//Player player(0.0, 0.0, 0.1, 10.0);

    double vertPos = 0.0;

    double angle = 30.0;
    double lastFrame = glfwGetTime();

    AABB cubeBounds
    {
        {-5.0, -35.0},
        {5.0, -25.0}
    };

    vector<AABB> obstacles =
    {
        cubeBounds,
        {
            {15.0, -45.0},
            {25.0, -35.0}
        },
        {
            {-25.0, -55.0},
            {-15.0, -45.0}
        }
    };

    int tmpct = 0;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        closeWindow(window);

        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;

        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        Vector2 currPos = camera.getPos();

		camera.movement(window, deltaTime, obstacles);
        camera.boundsCheck(mapWidth, mapWidth);
		camera.jump(window);
		camera.applyGravity(deltaTime);
		camera.applyView();
        
        glColor3f(1.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(-mapWidth, -15.0, mapWidth);
        glVertex3f(mapWidth, -15.0, mapWidth);
        glVertex3f(mapWidth, -15.0, -mapWidth);
        glVertex3f(-mapWidth, -15.0, -mapWidth);
        glEnd();

        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-mapWidth /4, -14.9, mapWidth / 4);
        glVertex3f(mapWidth / 4, -14.9, mapWidth / 4);
        glVertex3f(mapWidth / 4, -14.9, -mapWidth / 4);
        glVertex3f(-mapWidth / 4, -14.9, -mapWidth / 4);
        glEnd();

        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();

		// Left wall
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(mapWidth, -10.0, -mapWidth);
        glVertex3f(mapWidth, -5.0, -mapWidth);
        glVertex3f(mapWidth, -5.0, mapWidth);
        glVertex3f(mapWidth, -10.0, mapWidth);
        glEnd();

        // Back wall
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-mapWidth, -10.0, -mapWidth);
        glVertex3f(-mapWidth, -5.0, -mapWidth);
        glVertex3f(mapWidth, -5.0, -mapWidth);
        glVertex3f(mapWidth, -10.0, -mapWidth);
        glEnd();

		// Right wall
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-mapWidth, -10.0, mapWidth);
        glVertex3f(-mapWidth, -5.0, mapWidth);
        glVertex3f(-mapWidth, -5.0, -mapWidth);
        glVertex3f(-mapWidth, -10.0, -mapWidth);
        glEnd();

		// Front wall
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-mapWidth, -10.0, mapWidth);
        glVertex3f(-mapWidth, -5.0, mapWidth);
        glVertex3f(mapWidth, -5.0, mapWidth);
        glVertex3f(mapWidth, -10.0, mapWidth);
        glEnd();

        double rotSpeed = 45.0;

        glPushMatrix();

        glTranslatef(0.0, vertPos, -30.0);
        drawCube(0.0, 0.0, 0.0, 10.0, angle);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(20.0, vertPos, -40.0);
        drawCube(0.0, 0.0, 0.0, 10.0, angle);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(-20.0, vertPos, -50.0);
        drawCube(0.0, 0.0, 0.0, 10.0, angle);

        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        angle += rotSpeed * deltaTime;
        vertPos = 2 * sin((pi / 2) * glfwGetTime()) + 1.0;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}