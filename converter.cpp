#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_ALL_COLOR_KEYWORDS	// Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION		// Expands implementation
#include "../nanosvg/src/nanosvg.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

void normalize(float x, float y, float maxWidth, float maxHeight, float* result) {
    result[0] = 2 * (x/maxWidth) - 1;
    result[1] = 1 - 2 * (y/maxHeight);

    std::cout << result[0] << ", " << result[1] << ",\n";
}

int main() {
    // Load
    struct NSVGimage* image;
    image = nsvgParseFromFile("rect.svg", "px", 96);
    printf("size: %f x %f\n", image->width, image->height);
    // Use...

    float result[24];
    int counter = 0;
    float maxWidth = image->width;
    float maxHeight = image->height;

    for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
        for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {
            for (int i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];
                //std::cout << "    Point: (" << p[0] << ", " << p[1] << ")\n";
                normalize(p[0], p[1], maxWidth, maxHeight, result + counter);
                counter += 2;
            }
        }
    }
    // Delete
    nsvgDelete(image);

    std::cout << "new\n\n";
    for (int x = 0; x < 24; x +=2) {
        std::cout << result[x] << ", " << result[x + 1] << "\n";
    }
}