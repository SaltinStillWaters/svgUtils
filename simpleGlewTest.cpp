#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();
    glFlush();
}

void initBuffers() {
    float positions[]  = {
        -1.f, 1.f,
         1.f, 1.f,
         0.f, -1.f,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
}

int main(int argcp, char** argv) {
    glutInit(&argcp, argv);
    glutCreateWindow("Window");

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew\n";
    }

    initBuffers();
    glutDisplayFunc(display);
    glutMainLoop();


    return 0;