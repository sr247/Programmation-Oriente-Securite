//
// Created by stephane on 10/4/17.
//

#ifndef PARSER_MP_IMAGE_H
#define PARSER_MP_IMAGE_H

#include <iostream>
#include <fstream>
#include <regex>
#include <cmath>
#include <iomanip>
using namespace std;


typedef struct Block{
    char type;
    unsigned int total_len;
    unsigned int content_size;
    string content;
}Block;



class Image{
public:
        string magic_number;
        unsigned int rows;
        unsigned int cols;
        int pixel_type;
        vector<Block> blocks;
        string data;
public:

    Image(const char* filename) : rows(0) , cols(0), pixel_type(-1){
        ifstream f;
        string s;
        f.open(filename, ios::binary); // TODO:Ici spécifier les droits

//        Ouverture du fichier et chargement intégrale dans une chaine "s" puis fermeture
        if(f.is_open())
        {
#ifdef DEBUG
            cout << "f.is_open()" << endl;
#endif

            while(f.tellg() < 8){
                char c;
                c = char(f.get());
                s.push_back(c);
#ifdef DEBUG
                cout << "f.tellg() < 8" << endl;
#endif
            }
            if(s == "Mini-PNG"){
                magic_number = s;
                cout << magic_number << endl;
                while(!f.eof()){
#ifdef DEBUG
                    cout << "!f.eof()" << endl;
#endif
                    Block b;

                    if(!read_bloc(b, f)){
                        cout << "ERROR::FILE::Corrupted !" << endl;
                        exit(-1);
                    }
#ifdef DEBUG
                    cout << "Current " ;
                    cout << "Bloc: " << b.type << endl
                         << "total: " << b.total_len << endl
                         << "longueur des données: " << b.content_size << endl
                         << "contenu: " << b.content << endl;
#endif
                    blocks.push_back(b);
                }
                print_bloc_list();
            }
            else{
                cout << "ERROR::FILE::Structure not match !" << endl;
                exit(-1);
            }
        }
        else
        {
            cout << "ERROR::FILE::Fail to open file !" << endl;
            exit(-1);
        }

        bool has_header = false;
        for(Block b : blocks){
            has_header |= b.type == 'H';
        }
        if(!has_header){
            cout << "ERROR::FILE::No header block Found!" << endl;
            exit(-1);
        }
    }

private:

    int read_bloc(Block& b, ifstream& f){
#ifdef DEBUG
        cout << "read_bloc()" << endl;
#endif
        char c;
        unsigned int total = 0;
        c = char(f.get());
        if(c == 'H'){
            cout << endl << "c == H" << endl;
            b.type = c;
            int len_block[4]={0};

            for(auto k : {0, 1, 2, 3}){
                len_block[k] = f.get();
                cout << len_block[k];
                total++;
            }
            b.content_size = hexToInt(len_block);
            cout << endl << "b.content_size:" << b.content_size << endl;

            int htoi[b.content_size] = { 0 };
            for(unsigned int k = 0; k < b.content_size; k++){
                htoi[k] = f.get();
                cout << htoi[k];
                b.content.push_back(char(htoi[k]));
                total++;
            }
            b.total_len = total;

            this->cols = hexToInt(htoi, 4, 0);
            this->rows = hexToInt(htoi, 4, 4);
            this->pixel_type = hexToInt(htoi, 1, 8);
        }
        else if(c == 'C'){
            cout << endl << "c == C" << endl;
            b.type = c;
            int len_block[4]={0};

            for(auto k : {0, 1, 2, 3}){
                len_block[k] = f.get();
                cout << len_block[k];
                total++;
            }
            b.content_size = hexToInt(len_block);
            cout << endl << "b.content_size:" << b.content_size << endl;

            for(unsigned int k = 0; k < b.content_size; k++){
                b.content.push_back(char(f.get()));
                total++;
            }
            cout << "\"" << b.content << "\"" << endl;
            b.total_len = total;

        }else if(c == 'D'){
            cout  << endl << "c == D" << endl;
            b.type = c;
            int len_block[4]={0};

            for(auto k : {0, 1, 2, 3}){
                len_block[k] = f.get();
                cout << len_block[k];
                total++;
            }
            b.content_size = hexToInt(len_block);
            cout << endl << "b.content_size:" << b.content_size << endl;

            int htoi[b.content_size] = { 0 };
            for(unsigned int k = 0; k < b.content_size; k++){
                htoi[k] = f.get();
                cout << htoi[k];
                b.content.push_back(char(htoi[k]));
                total++;
            }
            b.total_len = total;


        }else{
            cout << endl << "c == ?" << endl;
            return 0;
        }
        return 1;
    }

    void print_bloc_list(){
        for(Block b : blocks){
            cout << "Bloc: " << b.type << endl
                 << "total: " << b.total_len << endl
                 << "longueur des données: " << b.content_size << endl
                 << "contenu: " << b.content << endl;
        }
    }


    // Tout ce qui concerne la représentation de l'image soit en console soit en fenêtre
public:

    /**
     *
     * @return
     */
    int to_string(){
        cout << "File type: " << magic_number << endl
             << "Dimension: " << cols << "x" << rows << endl
             << "Shading: "<< pixel_type << endl;
        return 0;
    }

    /**
     * Affiche tous les commentaires contenus dans
     * le fichier.
     *
     * Notes: Utilise le code coloration ANSI pour distinguer les commantaires du reste du texte
     *
     */
    void show_comment(){
            cout << "\033[38;2;205;205;205m\033[48;2;75;75;75m"
                 << ":Comments:" << "\033[0m" << endl;
            for(Block b : blocks){
                if(b.type == 'C'){
                    cout << "\033[38;2;205;205;205m\033[48;2;75;75;75m" << "\"" << b.content << "\""<< "\033[0m" << endl;
                }
            }
        cout << "\033[38;2;205;205;205m\033[48;2;75;75;75m"
             << ":End:" << "\033[0m" << endl;
    }

    /**
     * Question 4: En essai: Affiché le contenu de data apres l'avoir traduit en binary
     */
    void show_ascii(){

//        for(char e : data)
//            cout << setw(4) << setfill('0') << hex
//                 << static_cast<int>(e) << endl;
//
//        vector<string> bitmap = hexToBin(this->data, 0, 4);
//        for(unsigned int i = 0; i < rows; i++) {
//            for(unsigned int j = 0; j < cols; j++) {
//                cout << ' ' << bitmap[i][j] << ' ';
//            }
//            cout << endl;
//        }

    }

    /**
     * Convertion hexadécimal vers décimal integer
     * @param conv   : le nombre à convertir
     * @param stride : le nombre de digit à parcourir
     * @return
     */
    unsigned int hexToInt(const int conv[], unsigned int stride = 4, unsigned int cursor = 0){
        unsigned int l = 0;
        for(unsigned int i = 0; i < stride; i++){
            l += (conv[cursor + i]) * pow(16, ((stride - 1) - i));
        }
        return l;
    }



    // TODO:Convertir data directement lors du parsing en chaines de bits 0;1
    /**
     * Convertir de hexadécimale vers binaire sur regroupement de 2⁴ (2^4)
     *
     * Notes: Utilise la fonction hexToInt.
     * @param s
     * @param pos
     * @param stride
     * @return
//     */
//    vector<string> hexToBin(const string& s, unsigned int pos, unsigned int stride = 4){
//
//        vector<string> bin;
//        unsigned int k = s.length();
//        unsigned int l = hexToInt(s, 0, k);
//        int a;
//
//        a = l;
//        for(unsigned int i = 0; i < cols; i++) {
//            string b;
//            for (unsigned int j = 0; j < rows; j++) {
//                a = a - int(pow(2, double(stride-j)));
//                if(a >= 0){
//                    b.push_back(' ');
//                } else
//                {
//                    b.push_back('x');
//                }
//
//            }
//            bin.push_back(b);
//        }
//        return bin;
//    }


    /**
     * Plus tard comme CV::imgshow()
     */
    void show(){};
};
#endif //PARSER_MP_IMAGE_H
