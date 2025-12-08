#include "Header/Input.h"
#include "Header/State.h"
#include <cmath>

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

            float nx = mx / w * 2 - 1;
            float ny = 1 - (my / h * 2);

            float dx = nx - lampX;
            float dy = ny - lampY;

            if (sqrt(dx * dx + dy * dy) < lampR)
                klimaOn = !klimaOn;
        }
    }
    else
        clickHandled = false;
}
