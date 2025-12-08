#include "../Header/Util.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../Header/stb_image.h"

int endProgram(std::string message)
{
    std::cout << message << std::endl;
    glfwTerminate();
    return -1;
}

unsigned int compileShader(GLenum type, const char* sourcePath)
{
    std::ifstream file(sourcePath);
    if (!file.is_open()) {
        std::cout << "Greska: ne mogu da otvorim sejder fajl: " << sourcePath << "\n";
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    const char* src = code.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char info[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cout << "Shader compile error ("
            << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
            << "):\n" << info << "\n";
    }

    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vsSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    char info[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, info);
        std::cout << "Shader program link error:\n" << info << "\n";
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned loadImageToTexture(const char* filePath)
{
    int w, h, ch;
    unsigned char* data = stbi_load(filePath, &w, &h, &ch, 0);

    if (!data)
    {
        std::cout << "Ne mogu ucitati teksturu: " << filePath << "\n";
        return 0;
    }

    stbi__vertical_flip(data, w, h, ch);

    GLenum format = (ch == 4 ? GL_RGBA : GL_RGB);

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return tex;
}

GLFWcursor* loadImageToCursor(const char* filePath)
{
    int w, h, ch;
    unsigned char* data = stbi_load(filePath, &w, &h, &ch, 0);

    if (!data)
    {
        std::cout << "Ne mogu ucitati kursor: " << filePath << "\n";
        return nullptr;
    }

    GLFWimage img;
    img.width = w;
    img.height = h;
    img.pixels = data;

    GLFWcursor* cursor = glfwCreateCursor(&img, w / 5, h / 5);

    stbi_image_free(data);
    return cursor;
}
