#include "OpenGLVisualizer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Camera state
float camDistance = 120.0f;
float rotX = 30.0f;
float rotY = 0.0f;
float panX = 0.0f;
float panY = 0.0f;

// Mouse state
bool dragging = false;
double lastMouseX, lastMouseY;

// Mouse move callback (rotate)
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!dragging) return;

    float dx = (float)(xpos - lastMouseX);
    float dy = (float)(ypos - lastMouseY);

    rotY += dx * 0.4f;
    rotX += dy * 0.4f;

    lastMouseX = xpos;
    lastMouseY = ypos;
}

// Mouse click callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            dragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else {
            dragging = false;
        }
    }
}

// Mouse scroll zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camDistance -= yoffset * 4.0f;
    if (camDistance < 10) camDistance = 10;
    if (camDistance > 500) camDistance = 500;
}

void OpenGLVisualizer::drawPath3D(const std::vector<GCommand>& commands) {

    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(1100, 850, "3D CNC Simulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // ---- PRINT CONTROLS TO CONSOLE ----
    std::cout << "\n=== 3D CNC Simulator Controls ===\n";
    std::cout << "Mouse Drag      = Rotate Camera\n";
    std::cout << "Mouse Scroll    = Zoom\n";
    std::cout << "W / S           = Zoom In / Out\n";
    std::cout << "Arrow Keys      = Pan\n";
    std::cout << "Q / E           = Rotate X\n";
    std::cout << "A / D           = Rotate Y\n";
    std::cout << "=================================\n\n";

    while (!glfwWindowShouldClose(window)) {

        // ---- ROTATION ----
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rotY -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rotY += 1.0f;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotX -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotX += 1.0f;

        // ---- PAN ----
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) panY += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) panY -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) panX -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) panX += 1.0f;

        // ---- KEYBOARD ZOOM ----
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camDistance -= 1.5f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camDistance += 1.5f;

        // Clamp zoom
        if (camDistance < 10) camDistance = 10;
        if (camDistance > 500) camDistance = 500;

        // ---- CLEAR SCREEN ----
        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---- CAMERA ----
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1100.0 / 850.0, 0.1, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(panX, panY, -camDistance);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);

        // ---- AXIS ----
        // glBegin(GL_LINES);
        // glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(80,0,0); // X
        // glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,80,0); // Y
        // glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,80); // Z
        // glEnd();
        
// ---- TOOLPATH ----
glColor3f(1, 1, 1);

float lastX = 0, lastY = 0, lastZ = 0;
bool hasLast = false;

for (auto& cmd : commands) {

    // G0 = move (no draw)
    if (cmd.code == 'G' && cmd.number == 0) {
        lastX = cmd.x;
        lastY = cmd.y;
        lastZ = cmd.z;
        hasLast = true;
        continue;
    }

    // G1 = straight line
    if (cmd.code == 'G' && cmd.number == 1 && hasLast) {

        glBegin(GL_LINES);
        glVertex3f(lastX, lastY, lastZ);
        glVertex3f(cmd.x, cmd.y, cmd.z);
        glEnd();

        lastX = cmd.x;
        lastY = cmd.y;
        lastZ = cmd.z;
        continue;
    }

    // G2 / G3 = ARC
    if (cmd.code == 'G' && (cmd.number == 2 || cmd.number == 3)) {

        float sx = lastX;
        float sy = lastY;

        float ex = cmd.x;
        float ey = cmd.y;

        float cx = sx + cmd.i;
        float cy = sy + cmd.j;

        float radius = sqrt((sx - cx)*(sx - cx) + (sy - cy)*(sy - cy));

        float startAngle = atan2(sy - cy, sx - cx);
        float endAngle   = atan2(ey - cy, ex - cx);

        bool clockwise = (cmd.number == 2);

        // Angle wrapping fix
        if (clockwise && endAngle > startAngle)
            endAngle -= 2 * M_PI;

        if (!clockwise && endAngle < startAngle)
            endAngle += 2 * M_PI;

        int segments = 180;
        float step = (endAngle - startAngle) / segments;

        glBegin(GL_LINE_STRIP);

        for (int i = 0; i <= segments; i++) {
            float angle = startAngle + step * i;

            float nx = cx + cos(angle) * radius;
            float ny = cy + sin(angle) * radius;

            glVertex3f(nx, ny, lastZ);
        }

        glEnd();

        lastX = ex;
        lastY = ey;
    }
}


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
