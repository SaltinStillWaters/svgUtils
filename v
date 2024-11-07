
#include <iostream>
#include <GL/freeglut.h>

struct Pair {
    float x;
    float y;
};

Pair convertSVGToGL(float svgX, float svgY, float minX = 0, float minY = 0, float width = 175, float height = 124) {
    float glX = (2.0f * (svgX - minX) / width) - 1.0f;
    float glY = 1.0f - (2.0f * (svgY - minY) / height);

    Pair p;
    p.x = glX;
    p.y = glY;

    return p;
}

int main() {
    float positions[] = {
        93.521347               , 21.174644,
        93.521347 + 50.04916    , 21.174644,
        93.521347               , 21.174644 + 41.065975,
        93.521347 + 50.04916    , 21.174644 + 41.065975,
    };

    for (int x = 0; x < sizeof(positions) / sizeof(positions[0]); x += 2) {
        Pair p = convertSVGToGL(positions[x], positions[x + 1]);

        positions[x] = p.x;
        positions[x + 1] = p.x;

        std::cout << positions[x] << ", " << positions[x + 1] << '\n';
    }

    return 0;
}

