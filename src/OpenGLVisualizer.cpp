#include "OpenGLVisualizer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void OpenGLVisualizer::drawPath3D(const std::vector<GCommand>& commands) {

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(1000, 800, "3D CNC Toolpath Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1000.0 / 800.0, 0.1, 500);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Camera position
        glTranslatef(0, 0, -80);
        glRotatef(30, 1, 0, 0);
        glRotatef((float)glfwGetTime() * 10, 0, 0, 1);

        // Draw Axis
        glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(50,0,0);
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,50,0);
        glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,50);
        glEnd();

        // Draw Toolpath
        glColor3f(0, 1, 0);
        glBegin(GL_LINE_STRIP);

        for (auto& cmd : commands) {
            glVertex3f(cmd.x, cmd.y, cmd.z);
        }

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
