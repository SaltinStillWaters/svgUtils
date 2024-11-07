#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_ALL_COLOR_KEYWORDS	// Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION		// Expands implementation
#include "../nanosvg/src/nanosvg.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <sstream>


void display();
void initBuffers();

float result[55];
int counter = -5;

struct ShaderProgramSource {
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    ShaderType shaderType = ShaderType::NONE;
    std::stringstream ss[2];

    std::string line;
    while (getline(stream, line)) {
        if (line.find("shader") != std::string::npos) {
            std::cout << "shader found!\n";
            if (line.find("vertex") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
                std::cout << "vertex found!\n";
            } else if (line.find("fragment") != std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
                std::cout << "fragment found!\n";
            }
        } else {
            ss[(int) shaderType] << line << '\n';
        }
    }
    std::cout << ss[0].str() << "\n\nEND LINE\n\n";

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(GLuint type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
        std::cout << message << '\n';
        
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void normalize(float x, float y, float maxWidth, float maxHeight, float* result, float r, float g, float b) {
    result[0] = 2 * (x/maxWidth) - 1;
    result[1] = 1 - 2 * (y/maxHeight);
    result[2] = r;
    result[3] = g;
    result[4] = b;

    std::cout << result[0] << ", " << result[1] << ", " << result[2] << ", " << result[3] << ", " << result[4] << ",\n";
}

int main(int argcp, char** argv) {
    // Load
    struct NSVGimage* image;
    image = nsvgParseFromFile("rect.svg", "px", 96);
    printf("size: %f x %f\n", image->width, image->height);
    // Use...

    
    float maxWidth = image->width;
    float maxHeight = image->height;

    for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {

        float a, r, g, b;
        if (shape->fill.type == NSVG_PAINT_COLOR) {
            unsigned int color = shape->fill.color;
            std::cout << "COLOR: " << color << "\n\n";

            a = ((color >> 24) & 0xFF) /255.0f;
            r = ((color >> 16) & 0xFF) / 255.0f;
            g = ((color >> 8) & 0xFF) / 255.0f;
            b = (color & 0xFF) / 255.0f;
        }

        for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
            for (int i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];
                //std::cout << "    Point: (" << p[0] << ", " << p[1] << ")\n";
                counter += 5;
                normalize(p[0], p[1], maxWidth, maxHeight, result + counter, r, g, b, a);
            }
        }
    }
    counter += 5;
    
    // Delete
    nsvgDelete(image);
    

    glutInit(&argcp, argv);
    glutCreateWindow("Window");

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew\n";
    }

    initBuffers();
    glutDisplayFunc(display);
    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, counter / 2);
    glutSwapBuffers();
    glFlush();
}

void initBuffers() {
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(result), result, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void *) (2 * sizeof(GLfloat)));

    ShaderProgramSource source = ParseShader("shader.shader");

    unsigned int shader = createShader(source.VertexShader, source.FragmentShader);
    glUseProgram(shader);

    glDeleteProgram(shader);
}