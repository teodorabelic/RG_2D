#pragma once
#include <GL/glew.h>

unsigned int LoadTexture(const char* path);
void InitTextureQuad();
void DrawTexture(unsigned int shader, unsigned int tex, float x, float y, float w, float h, float rotation, float scale);
