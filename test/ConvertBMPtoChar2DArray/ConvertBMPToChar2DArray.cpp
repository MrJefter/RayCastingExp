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

            if (color.red == 0) myfile << "#";
            else if (color.red == 255 && color.blue == 0) myfile << "M";
            else myfile << ".";
        }
        myfile << "\"," << endl;
    }
    myfile << "};";
}
