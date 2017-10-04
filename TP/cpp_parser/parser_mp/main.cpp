#include <iostream>
#include "Image.h"


using namespace std;

static int to_string(const Image &img){
    cout << "File type:" << img.type << endl;
    std::cout << "L :" << img.cols << endl;
    cout << "H: " << img.rows << endl;
    cout << "Shading :"<< img.pixel_type << endl;
    return 0;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    Image img("../data/pass_file/A.mp");
    return 0;
}