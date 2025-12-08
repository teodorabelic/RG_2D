#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "../Header/Util.h"
#include "../Header/Rectangle.h"
#include "../Header/Circle.h"
#include "../Header/Input.h"
#include "../Header/State.h"
#include "../Header/Texture.h"
#include "../Header/stb_image.h"

// cifre, ikonice i kursor
unsigned int digits[10];
unsigned int iconFire, iconSnow, iconOK, iconMinus, iconCelsius;
unsigned int iconCreator;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // full screen
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(
        mode->width, mode->height,
        "Air Conditioner",
        glfwGetPrimaryMonitor(), NULL);

    if (!window) return endProgram("Prozor nije kreiran.");
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        return endProgram("GLEW neuspesan.");

    // cursor daljinski upravljac
    int cw, ch, cc;
    unsigned char* cursorData = stbi_load("Textures/icons/cursor.png", &cw, &ch, &cc, 4);

    if (cursorData)
    {
        GLFWimage img;
        img.width = cw;
        img.height = ch;
        img.pixels = cursorData;

        // hotspot - gornji levi ugao
        GLFWcursor* cursor = glfwCreateCursor(&img, 10, 10);
        glfwSetCursor(window, cursor);

        stbi_image_free(cursorData);
    }
    else
    {
        printf("FAILED TO LOAD CURSOR TEXTURE!\n");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    unsigned int shader = createShader("basic.vert", "basic.frag");

    InitRectangle();
    InitCircle();
    InitTextureQuad();

    // ucitavanje cifara 
    for (int i = 0; i < 10; i++)
    {
        char path[64];
        snprintf(path, sizeof(path), "Textures/digits/%d.png", i);
        digits[i] = LoadTexture(path);
    }

    // ikonice
    iconFire = LoadTexture("Textures/icons/fire.png");
    iconSnow = LoadTexture("Textures/icons/snow.png");
    iconOK = LoadTexture("Textures/icons/ok.png");
    iconMinus = LoadTexture("Textures/icons/minus.png");
    iconCelsius = LoadTexture("Textures/icons/celsius.png");
    iconCreator = LoadTexture("Textures/icons/creator.png");

    double lastTime = glfwGetTime();
    int frames = 0;

    const double targetFrameTime = 1.0 / 75.0; // ~0.01333 sec

    while (!glfwWindowShouldClose(window))
    {
        double frameStart = glfwGetTime();

        frames++;
        double now = glfwGetTime();

        if (now - lastTime >= 1.0) {
            //printf("FPS: %d\n", frames);
            frames = 0;
            lastTime = now;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        HandleLampClick(window);
        HandleKeyboardInput(window);
        UpdateState();
        if (klimaOn) UpdateTemp();

        // klima
        DrawRectangle(shader, 0, 0.60f, 1.4f, 0.40f, 1, 1, 1);

        // lampica
        DrawCircle(shader, lampX, lampY, lampR, klimaOn ? 1.f : 0.3f, klimaOn ? 0.f : 0.3f, klimaOn ? 0.f : 0.3f);

        // rupa
        DrawRectangle(shader, 0, 0.50f, 1.2f, 0.10f, 0.3f, 0.3f, 0.3f);

        // precaga
        float precY = 0.50f + (0.10f - precagaHeight) / 2;
        DrawRectangle(shader, 0, precY, 1.2f, precagaHeight,
            0.7f, 0.7f, 0.7f);

        // ekrani - crni pravougaonici
        float screenC = klimaOn ? 0.0f : 0.0f;

        DrawRectangle(shader, -0.30f, 0.67f, 0.25f, 0.15f, screenC, screenC, screenC);
        DrawRectangle(shader, 0.00f, 0.67f, 0.25f, 0.15f, screenC, screenC, screenC);
        DrawRectangle(shader, 0.30f, 0.67f, 0.25f, 0.15f, screenC, screenC, screenC);

        // kada je upaljena klima
        if (klimaOn)
        {
            float y = 0.67f;

            float digitW = 0.10f * 0.5f;
            float digitH = 0.16f * 0.5f;

            float rot = -0.35f;
            float scl = 0.75f;

            // levi ekran (zeljena temperatura)
            int tempD = desiredTemp;
            float xStart = -0.32f;

            if (tempD < 0)
            {
                DrawTexture(shader, iconMinus, xStart - 0.04f, y, digitW * 0.55f, digitH * 0.55f, rot, 0.70f);
                tempD = -tempD;
            }

            int d1 = tempD / 10;
            int d2 = tempD % 10;

            DrawTexture(shader, digits[d1], xStart, y, digitW, digitH, rot, scl);
            DrawTexture(shader, digits[d2], xStart + 0.04f, y, digitW, digitH, rot, scl);

            DrawTexture(shader, iconCelsius, xStart + 0.08f, y + 0.02f, digitW * 0.55f, digitH * 0.55f, rot, 0.70f);

            // srednji ekran (trenutna temperatura)
            int tempM = currentTemp;
            float x2 = -0.02f;

            if (tempM < 0)
            {
                DrawTexture(shader, iconMinus, x2 - 0.04f, y, digitW * 0.55f, digitH * 0.55f, rot, 0.70f);
                tempM = -tempM;
            }

            int m1 = tempM / 10;
            int m2 = tempM % 10;

            DrawTexture(shader, digits[m1], x2, y, digitW, digitH, rot, scl);
            DrawTexture(shader, digits[m2], x2 + 0.04f, y, digitW, digitH, rot, scl);

            DrawTexture(shader, iconCelsius, x2 + 0.08f, y + 0.02f, digitW * 0.55f, digitH * 0.55f, rot, 0.70f);

            // desni ekran - ikonice
            unsigned int icon = iconOK;
            if (desiredTemp > currentTemp) icon = iconFire;
            else if (desiredTemp < currentTemp) icon = iconSnow;

            DrawTexture(shader, icon, 0.30f, y,
                0.07f, 0.10f, rot, 0.70f);
        }

        // lavor i voda
        DrawRectangle(shader, 0.0f, -0.80f, 0.40f, 0.20f, 1, 1, 1);

        float waterY = -0.80f - 0.10f + waterHeight / 2.0f;
        DrawRectangle(shader, 0.0f, waterY, 0.36f, waterHeight,
            0.4f, 0.6f, 1.0f);

        // potpis
        DrawTexture(shader, iconCreator, -0.85f, -0.85f, 0.30f,0.30f, 0.0f, 1.0f);

        glfwSwapBuffers(window);

        double frameEnd = glfwGetTime();
        double elapsed = frameEnd - frameStart;

        if (elapsed < targetFrameTime) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(targetFrameTime - elapsed)
            );
        }

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
