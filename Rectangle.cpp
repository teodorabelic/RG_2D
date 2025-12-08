#include <GL/glew.h>
#include "Header/Rectangle.h"

static unsigned int vao, vbo, ebo;

void InitRectangle()
{
    float verts[] = {
        // pos(x,y)     color(r,g,b)
        -0.5f, -0.5f,   1,1,1,
         0.5f, -0.5f,   1,1,1,
         0.5f,  0.5f,   1,1,1,
        -0.5f,  0.5f,   1,1,1
    };

    unsigned int idx[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    // POSITION → layout(location=0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // COLOR → layout(location=2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void DrawRectangle(unsigned int shader, float x, float y, float w, float h,
    float r, float g, float b)
{
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "useTexture"), false);
    glUniform3f(glGetUniformLocation(shader, "solidColor"), r, g, b);

    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uSize"), w, h);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
