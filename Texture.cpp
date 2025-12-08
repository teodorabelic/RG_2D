#include "Header/Texture.h"
#include "Header/stb_image.h"
#include <GL/glew.h>
#include <cstdio>

static unsigned int texVAO = 0, texVBO = 0, texEBO = 0;

unsigned int LoadTexture(const char* path)
{
    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 4);

    if (!data)
    {
        printf("FAILED TO LOAD: %s\n", path);
        return 0;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
    return tex;
}

void InitTextureQuad()
{
    if (texVAO != 0) return;

    float verts[] = {
        //   x      y       u   v     r g b
        -0.5f, -0.5f,   0,1,   1,1,1,   // v = 1 (bilo 0) - rotacija slike
         0.5f, -0.5f,   1,1,   1,1,1,
         0.5f,  0.5f,   1,0,   1,1,1,
        -0.5f,  0.5f,   0,0,   1,1,1
    };

    unsigned int idx[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &texVAO);
    glGenBuffers(1, &texVBO);
    glGenBuffers(1, &texEBO);

    glBindVertexArray(texVAO);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void DrawTexture(unsigned int shader, unsigned int tex,
    float x, float y, float w, float h,
    float rotation, float scale)
{
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "useTexture"), true);

    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform2f(glGetUniformLocation(shader, "uSize"), w, h);

    GLint locRot = glGetUniformLocation(shader, "uRotation");
    GLint locScale = glGetUniformLocation(shader, "uScale");
    if (locRot != -1)   glUniform1f(locRot, rotation);
    if (locScale != -1) glUniform1f(locScale, scale);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);

    glBindVertexArray(texVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texEBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
