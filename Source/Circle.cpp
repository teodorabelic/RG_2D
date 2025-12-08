#include "Header/Circle.h"
#include <vector>
#include <cmath>

static unsigned int circleVAO, circleVBO;
static int circleVertexCount;

void InitCircle()
{
    const int segments = 40;
    circleVertexCount = segments + 2;

    std::vector<float> verts;
    verts.reserve(circleVertexCount * 2);

    verts.push_back(0.0f);
    verts.push_back(0.0f);

    for (int i = 0;i <= segments;i++)
    {
        float angle = i * 2.0f * 3.14159f / segments;
        verts.push_back(cos(angle));
        verts.push_back(sin(angle));
    }

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float),
        verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void DrawCircle(unsigned int shader, float x, float y, float radius,
    float r, float g, float b)
{
    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uSize"), radius, radius);
    glUniform3f(glGetUniformLocation(shader, "uColor"), r, g, b);

    glBindVertexArray(circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertexCount);
}
