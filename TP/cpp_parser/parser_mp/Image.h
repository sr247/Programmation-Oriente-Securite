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
        char *type_file;
        int rows;
        int cols;
        char *pixel_type;
public:

    Image(const char* filename){
        ifstream f;
        string s;
        f.open(filename); // Ici spécifier les droits
        if(f.is_open())
        {
            while(!f.eof()){
                string line;
                getline(f, line);
                s.append(line);
            }
            f.close();
        }
        else
        {
            cout << "Fail to open file" << endl;
            exit(-1);
        }

        cout << s << s.length() << endl;

        string tf(s.substr(0, 8));

        if(tf == "Mini-PNG"){
            type_file = new char[9];
            tf.copy(type_file, 8);
            type_file[8] = '\0';
            cout << type_file << endl;

            unsigned int i = 8;
                if(s[i] == 'H'){
                    int t;

                    const char* p = s.substr(i, i+8).cbegin();
//                    const char* p = "10 200000000000000000000000000000 30 -40";
                    char *end;
                    std::cout << "Parsing '" << p << "':\n";
                    for (long i = std::strtol(p, &end, 10);
                         p != end;
                         i = std::strtol(p, &end, 10))
                    {
                        std::cout << "'" << std::string(p, end-p) << "' -> ";
                        p = end;
                        if (errno == ERANGE){
                            std::cout << "range error, got ";
                            errno = 0;
                        }
                        std::cout << i << '\n';
                    }
//                    for (long i = std::strtol(ss, &end, 10);
//                         ss != end;
//                         i = std::strtol(ss, &end, 10)){
//
//                        std::cout << "'" << std::string(s, end-ss) << "' -> ";
//                        ss = end;
//                        if (errno == ERANGE){
//                            std::cout << "range error, got ";
//                            errno = 0;
//                        }
//                        std::cout << i << '\n';
//
//                    }
                    /*
                    t = strtol(ss, end);
                    cout << t << endl;

                    i += 4;
                    ss = s.substr(i, i+4);
                    t = strtol(ss.c_str(),);
                    cout << t << endl;

                    i += 4;
                    ss = s.substr(i, i+4);
                    t = atoi(ss.c_str());
                    cout << t << endl;
                     */
                }
                else if(s[i] == 'C'){

                }
                else if(s[i] == 'D'){

                }
                else
                {

                }


        }else
        {
            cout << "Fichier non conforme" << endl;
            exit(-1);
        }



    }

//
//    Image(const char* filename){
//        ifstream f;
//        string s;
//        f.open(filename); // Ici spécifier les droits
//        if(f.is_open())
//        {
//            while(!f.eof()){
//                string line;
//                f >> line;
//                cout << line << endl;
//                s.append(line);
//            }
//            f.close();
//        }
//        else {
//            cout << "Fail to open file" << endl;
//            exit(-1);
//        }
//
//    }
//

    Image(const string filename){
        ofstream f(filename.c_str());

    }

};
#endif //PARSER_MP_IMAGE_H
