#include "Header/Input.h"
#include "Header/State.h"
#include <cmath>

// lampica
bool klimaOn = false;
bool clickHandled = false;

float lampX = 0.65f;
float lampY = 0.50f;
float lampR = 0.02f;

// lavor
extern float waterHeight;

// promena temperature
static bool upWasPressed = false;
static bool downWasPressed = false;


void HandleLampClick(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!clickHandled)
        {
            clickHandled = true;

            double mx, my;
            glfwGetCursorPos(window, &mx, &my);

            int w, h;
            glfwGetWindowSize(window, &w, &h);

            float nx = float(mx) / w * 2.f - 1.f;
            float ny = 1.f - float(my) / h * 2.f;

            float dx = nx - lampX;
            float dy = ny - lampY;

            if (dx * dx + dy * dy < lampR * lampR)
                klimaOn = !klimaOn;
        }
    }
    else
    {
        clickHandled = false;
    }
}

void HandleKeyboardInput(GLFWwindow* window)
{
    // space
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        waterHeight = 0.0f;
    }

    // strelica gore
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (!upWasPressed)
        {
            if (desiredTemp < 40)
            {
                desiredTemp++;
            }
            upWasPressed = true;
        }
    } else {
        upWasPressed = false;
    }

    //strelica dole
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (!downWasPressed) {
            if (desiredTemp > -10)
            {
                desiredTemp--;
            }
            downWasPressed = true;
        }
    }
    else {
        downWasPressed = false;
    }
}
