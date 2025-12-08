#include <GL/glew.h>
#include <vector>
#include <cmath>
#include "Header/Circle.h"

static unsigned int circleVAO, circleVBO;
static int vertexCount;

void InitCircle()
{
    const int segments = 40;
    vertexCount = segments + 2;

    std::vector<float> verts;
    verts.reserve(vertexCount * 7);

    // center
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    verts.push_back(0.5f);
    verts.push_back(0.5f);
    verts.push_back(1);
    verts.push_back(1);
    verts.push_back(1);

    for (int i = 0; i <= segments; i++)
    {
        float angle = float(i) / segments * 6.28318f;
        float x = cos(angle);
        float y = sin(angle);

        verts.push_back(x);
        verts.push_back(y);

        verts.push_back((x + 1) * 0.5f);
        verts.push_back((y + 1) * 0.5f);

        verts.push_back(1);
        verts.push_back(1);
        verts.push_back(1);
    }

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void DrawCircle(unsigned int shader, float x, float y, float r, float cr, float cg, float cb)
{
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "useTexture"), false);
    glUniform3f(glGetUniformLocation(shader, "solidColor"), cr, cg, cb);

    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uSize"), r, r);

    glBindVertexArray(circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}
