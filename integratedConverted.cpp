#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_ALL_COLOR_KEYWORDS	// Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION		// Expands implementation
#include "../nanosvg/src/nanosvg.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

void display();
void initBuffers();

float result[24];
int counter = -2;


void normalize(float x, float y, float maxWidth, float maxHeight, float* result) {
    result[0] = 2 * (x/maxWidth) - 1;
    result[1] = 1 - 2 * (y/maxHeight);

    std::cout << result[0] << ", " << result[1] << ",\n";
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
        for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
            for (int i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];
                //std::cout << "    Point: (" << p[0] << ", " << p[1] << ")\n";
                counter += 2;
                normalize(p[0], p[1], maxWidth, maxHeight, result + counter);
            }
        }
    }
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
    glDrawArrays(GL_QUADS, 0, 12);
    glutSwapBuffers();
    glFlush();
}

void initBuffers() {
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(result), result, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
}