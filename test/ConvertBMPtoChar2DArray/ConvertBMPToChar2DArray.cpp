#include <iostream>
#include <fstream>
#include "bitmap_image.h"

using namespace std;

int main() {
    bitmap_image image("input.bmp");

    if (!image) {
        printf("Error - Failed to open: input.bmp\n");
        return 1;
    }

    ofstream myfile;
    myfile.open ("array.txt");
    myfile << "char mapArray[" << image.width()
    << "][" << image.height() << "] = {" << endl;

    rgb_t color;

    for (int countY = 0; countY < image.height(); countY++) {
        myfile << "\"";
        for (int countX = 0; countX < image.width(); countX++) {
            image.get_pixel(countX, countY, color);

            if (color.green == 0) myfile << "\#";
            else myfile << ".";
        }
        myfile << "\"," << endl;
    }
    myfile << "};";
}
