//
// Created by stephane on 10/4/17.
//

#ifndef PARSER_MP_IMAGE_H
#define PARSER_MP_IMAGE_H

#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

class Image{
    public:
        string type;
        int rows;
        int cols;
        string pixel_type;

    Image(const char* filename){
        ifstream f(filename);
        if(f)
        {

        }
        else
        {
            cout << "Fail to open file" << endl;
        }


    }

    Image(const string filename){
        ofstream f(filename.c_str());

    }

};
#endif //PARSER_MP_IMAGE_H