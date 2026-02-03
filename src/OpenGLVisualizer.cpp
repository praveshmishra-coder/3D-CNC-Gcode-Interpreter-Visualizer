#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGLVisualizer.h"
#include "GLWindow.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// ---------------- CAMERA STATE ----------------
float camDistance = 120.0f;
float rotX = 30.0f;
float rotY = 0.0f;
float panX = 0.0f;
float panY = 0.0f;

// ---------------- MOUSE STATE ----------------
bool dragging = false;
double lastMouseX, lastMouseY;

// ---------------- PLAYBACK STATE ----------------
size_t playIndex = 0;
bool isPlaying = true;
double lastStepTime = 0.0;
double stepDelay = 0.03;

// ---------------- CALLBACKS ----------------
void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (!dragging) return;
    rotY += float(xpos - lastMouseX) * 0.4f;
    rotX += float(ypos - lastMouseY) * 0.4f;
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            dragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else {
            dragging = false;
        }
    }
}

void scroll_callback(GLFWwindow*, double, double yoffset) {
    camDistance -= float(yoffset) * 4.0f;
    camDistance = std::clamp(camDistance, 10.0f, 500.0f);
}

// ---------------- MAIN DRAW ----------------
void OpenGLVisualizer::drawPath3D(const std::vector<GCommand>& commands) {

    // ✅ RAII window
    GLWindow glWindow(1100, 850, "3D CNC Simulator");
    GLFWwindow* window = glWindow.get();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    std::cout <<
        "\n=== CNC Playback Controls ===\n"
        "SPACE  : Play / Pause\n"
        "RIGHT  : Step Forward\n"
        "R      : Restart\n"
        "Mouse  : Rotate\n"
        "Scroll : Zoom\n\n";

    while (!glfwWindowShouldClose(window)) {

        // -------- INPUT --------
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            isPlaying = !isPlaying;
            glfwWaitEventsTimeout(0.2);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            playIndex = std::min(playIndex + 1, commands.size());
            glfwWaitEventsTimeout(0.15);
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            playIndex = 0;
            isPlaying = true;
            glfwWaitEventsTimeout(0.2);
        }

        // -------- AUTO PLAY --------
        double now = glfwGetTime();
        if (isPlaying && now - lastStepTime > stepDelay) {
            if (playIndex < commands.size())
                playIndex++;
            lastStepTime = now;
        }

        // -------- CLEAR --------
        glClearColor(0.02f, 0.02f, 0.04f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // -------- CAMERA --------
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1100.0 / 850.0, 0.1, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(panX, panY, -camDistance);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);

        // -------- TOOLPATH --------
        glColor3f(1, 1, 1);

        float lastX = 0, lastY = 0, lastZ = 0;
        bool hasLast = false;

        for (size_t c = 0; c < playIndex; c++) {
            const auto& cmd = commands[c];

            if (cmd.code == 'G' && cmd.number == 0) {
                lastX = cmd.x; lastY = cmd.y; lastZ = cmd.z;
                hasLast = true;
            }

            if (cmd.code == 'G' && cmd.number == 1 && hasLast) {
                glBegin(GL_LINES);
                glVertex3f(lastX, lastY, lastZ);
                glVertex3f(cmd.x, cmd.y, cmd.z);
                glEnd();
                lastX = cmd.x; lastY = cmd.y; lastZ = cmd.z;
            }

            if (cmd.code == 'G' && (cmd.number == 2 || cmd.number == 3)) {
                float cx = lastX + cmd.i;
                float cy = lastY + cmd.j;

                float r = std::hypot(lastX - cx, lastY - cy);
                float a0 = atan2(lastY - cy, lastX - cx);
                float a1 = atan2(cmd.y - cy, cmd.x - cx);

                bool cw = (cmd.number == 2);
                if (cw && a1 > a0) a1 -= 2 * M_PI;
                if (!cw && a1 < a0) a1 += 2 * M_PI;

                glBegin(GL_LINE_STRIP);
                for (int i = 0; i <= 180; i++) {
                    float a = a0 + (a1 - a0) * i / 180.0f;
                    glVertex3f(cx + cos(a) * r, cy + sin(a) * r, lastZ);
                }
                glEnd();

                lastX = cmd.x;
                lastY = cmd.y;
                lastZ = cmd.z;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
