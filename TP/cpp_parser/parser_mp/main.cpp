#include <iostream>
#include "Image.h"


using namespace std;

int main(int argv, char ** args) {

    if(argv > 2){
        cout << "Too many arguments" << endl;
    }
    else {
        Image img(args[1]);
        img.to_string();
        img.show_comment();
        img.show_ascii();

    }


    return 0;
}