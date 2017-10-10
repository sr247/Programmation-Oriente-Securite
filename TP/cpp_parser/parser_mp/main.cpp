#include <iostream>
#include "Image.h"


using namespace std;

int main(int argv, char ** args) {

    if(argv > 2){
        cout << "Too many arguments" << endl;
    }
    else {
//        if(regex_match(args[1])){  // vérifier que le match du fichier soit = a *.mp regex
//
//        }
        Image img(args[1]);
        img.to_string();
        img.show_comment();
//        img.show_ascii();

    }


    return 0;
}