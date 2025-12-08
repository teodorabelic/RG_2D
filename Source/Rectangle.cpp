#include "Header/Rectangle.h"

static unsigned int vao, vbo, ebo;

void InitRectangle()
{
    float vertices[] = {
        -0.5f, -0.5f, 1,1,1,
         0.5f, -0.5f, 1,1,1,
         0.5f,  0.5f, 1,1,1,
        -0.5f,  0.5f, 1,1,1
    };

    unsigned int indices[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void DrawRectangle(unsigned int shader, float x, float y, float w, float h,
    float r, float g, float b)
{
    glUseProgram(shader);
    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uSize"), w, h);
    glUniform3f(glGetUniformLocation(shader, "uColor"), r, g, b);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
