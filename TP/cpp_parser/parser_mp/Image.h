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
#include <bitset>
using namespace std;


class Block
{
public:
    unsigned char type;
    unsigned int total_len;
    unsigned int content_size;
    vector<unsigned char> content;         // Ou string... Décidé duquel est mieux
    vector<string> bitmap;
public:
    Block(unsigned char t,
          unsigned int cs, vector<unsigned char> contenu,
          vector<string> bm = vector<string>()){
        type = t;
        content_size = cs;
        content = contenu;
        bitmap = bm;
        total_len = 1 + 4 + content_size;
    }

    Block() : type(0), total_len(0), content_size(0), content(), bitmap(){}
};

template<std::size_t B>
int bitset_to_long(std::bitset<B>& b) {
    struct {int x:B;} s;
    s.x = int(b.to_ulong());
    return s.x;
}

class Image{
public:
        string file_name;
        string magic_number;
        unsigned int rows;
        unsigned int cols;
        int pixel_type;
        vector<Block> blocks;
        vector<int> data;
public:

    Image() : file_name(), magic_number(), rows(0), cols(0), pixel_type(0), blocks(), data(){

    }

    Image(const char* filename) : rows(0) , cols(0), pixel_type(0){
        file_name = filename;
        ifstream f;
        string s;
        f.open(filename, ios::binary | ios::in);
//        Ouverture du fichier et chargement intégrale dans une chaine "s" puis fermeture
        if(f.is_open())
        {
#ifdef DEBUG
            cout << "f.is_open()" << endl;
#endif

            while(f.tellg() < 8){
                unsigned char c;
                c = (unsigned char)(f.get());
                s.push_back(c);
#ifdef DEBUG
                cout << "f.tellg() < 8" << endl;
#endif
            }
            if(s == "Mini-PNG"){
                magic_number = s;
                cout << magic_number << endl;
                while(!f.eof()){
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
                    if(!f.eof()){
                        blocks.push_back(b);
                    }
                }
                print_bloc_list();
            }
            else{
                cout << "ERROR::FILE::Format Not match !" << endl;
                exit(-1);
            }
        }
        else
        {
            cout << "ERROR::FILE::Fail to open file !" << endl;
            exit(-1);
        }

        bool has_header = false;
        int check_data_sum = 0;
        auto total_data_lenght = int(ceil(float(cols*rows)/8.));
        int nb_data = 0;
        for(Block b : blocks){
            has_header |= b.type == 'H';
            if(b.type == 'D'){
                nb_data++;
                check_data_sum += b.content_size;
            }
        }

        if(check_data_sum != total_data_lenght){
            cout << "ERROR::FILE::DATA::Lenght not uniform !" << endl;
            cout << check_data_sum << " " << int(ceil(float(cols*rows)/8.)) << endl;
            exit(-1);
        }

        if(nb_data <= 0){
            cout << "ERROR::FILE::DATA::Not found !" << endl;
            exit(-1);
        }

        if(!has_header){
            cout << "ERROR::FILE::HEADER::Not found !" << endl;
            exit(-1);
        }

        // ----------------------------------------------------------------------------------------
        // ----------------------------------------------------------------------------------------

        for(Block bb : blocks){
            if(bb.type == 'D'){
                for(unsigned char c : bb.content){
                    // TODO: Rajouter des tabulations pour éclairer le débug

                    // TODO: Choix retenu
                    // -Soit J'utilise data comme un liste
                    //  Et je l'affiche en sautant une ligne modulo cols

                    //TODO: Supprimer les parametres inutiles des fontion lorsque modifier
                    string s;
                    vector<int> vec;
                    int tmp[1] = { int(c) };
                    vec = intToBin(tmp);
                    for(int i : vec){
                        data.push_back(i);
                        s += std::to_string(i);
                    }
                    bb.bitmap.push_back(s);
                }
                cout << "bitmaps:";
                for(string& b : bb.bitmap){
                    cout << b << ' ';
                }
                cout << endl;
            }
        }

        cout << "data:";
        for(int val : data){
                cout << val;
        }
        cout << endl;




    }


    int save(){
        ifstream f;
        f.open(file_name);
        return 1;
    }
private:

    int read_bloc(Block& b, ifstream& f){
#ifdef DEBUG
        cout << "read_bloc()" << endl;
#endif
        unsigned char c;
        unsigned int total = 1;
        c = (unsigned char)(f.get());

        if(f.eof()){
            cout << endl << "eof" << endl;
            return 1;
        }else if(c == 'H'){
//            cout << endl << "c == H" << endl;
            b.type = c;
            int len_bloc[4]={0};

            for(auto k : {0, 1, 2, 3}){
                len_bloc[k] = f.get();
//                cout << len_bloc[k];
                total++;
            }
            b.content_size = hexToInt(len_bloc);
//            cout << endl << "b.content_size:" << b.content_size << endl;

            int htoi[b.content_size] = { 0 };
            for(unsigned int k = 0; k < b.content_size; k++){
                htoi[k] = f.get();
//                cout << htoi[k];
                b.content.push_back((unsigned char)(htoi[k]));
                total++;
            }
            cout << endl;
            b.total_len = total;

            this->cols = hexToInt(htoi, 4, 0);
            this->rows = hexToInt(htoi, 4, 4);
            this->pixel_type = hexToInt(htoi, 1, 8);
        }
        else if(c == 'C'){
//            cout << endl << "c == C" << endl;
            b.type = c;
            int len_bloc[4] = { 0 };

            for(auto k : {0, 1, 2, 3}){
                len_bloc[k] = f.get();
//                cout << len_bloc[k];
                total++;
            }
            b.content_size = hexToInt(len_bloc);
//            cout << endl << "b.content_size:" << b.content_size << endl;

            for(unsigned int k = 0; k < b.content_size; k++){
                b.content.push_back((unsigned char)(f.get()));
                total++;
            }

//            cout << "\"";
//            for(unsigned char t : b.content){
//               cout << t;
//            }
//            cout << "\"" << endl;

            b.total_len = total;

        }else if(c == 'D'){
//            cout  << endl << "c == D" << endl;
            b.type = c;
            int len_bloc[4] = { 0 };

            for(auto k : {0, 1, 2, 3}){
                len_bloc[k] = f.get();
//                cout << len_bloc[k];
                total++;
            }
            b.content_size = hexToInt(len_bloc);
//            cout << endl << "b.content_size:" << b.content_size << endl;

            int htoi[b.content_size] = { 0 };
            for(unsigned int k = 0; k < b.content_size; k++){
                htoi[k] = f.get();
//                cout << htoi[k] << ' ';
                b.content.push_back((unsigned char)(htoi[k]));
                total++;
            }
            b.total_len = total;

        }else{
            cout << endl << "c == ?:" << int(c) << endl;
            return 0;
        }
        return 1;
    }

    /**
     * TODO:Fonction à intégrer à la structure bloc
     */
    void print_bloc_list(){
        for(Block b : blocks){
            cout << "Bloc: " << b.type << endl
                 << "total: " << b.total_len << endl
                 << "longueur des données: " << b.content_size << endl
                 << "contenu:";
            for(char c : b.content){
                cout << c;
            }
            cout << endl;

        }
    }


    // Tout ce qui concerne la représentation de l'image soit en console soit en fenêtre
public:

    /**
     * Methode de description de l'image
     * @return
     */
    void to_string(){
        cout << "File type: " << magic_number << endl
             << "Dimension: " << cols << "x" << rows << endl
             << "Shading: "<< pixel_type << endl;
    }

    /**
     * Affiche tous les commentaires contenus dans
     * le fichier.
     *
     * Notes: Utilise le code coloration ANSI pour distinguer les commantaires du reste du texte
     *
     */
    void show_comment(){
            cout << "\033[38;2;192;240;1m\033[48;2;70;70;70m"
                 << ":Comments:" << "\033[0m" << endl;
            for(Block b : blocks){
                if(b.type == 'C'){
                    cout << "\033[38;2;192;240;1m\033[48;2;70;70;70m"<< "\"";
                    for(char c : b.content){
                        cout << c;
                    }
                    cout << "\""<< "\033[0m" << endl;
                }
            }
            cout << "\033[38;2;192;240;1m\033[48;2;70;70;70m"
                 << ":End:" << "\033[0m" << endl;
    }

    /**
     * Question 4: En essai: Affiché le contenu de data apres l'avoir traduit en binary
     */
    void show_ascii(){
        cout << "Affichage:" << endl;
        for(unsigned int i = 0; i < cols*rows; i++){
            if(i % cols == 0){
                cout << endl;
            }
            if(data[i] == 1){
                cout << ' ';
            }else{
                cout << 'x';
            }
        }
        cout << endl;
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


    static vector<int> intToHex(int a, unsigned int n = 4){
        vector<int> b;
//        @param
//        a = nombre à convertir
//        n = nombre de digit sur lequel coder
//        Selon la spécification pour le parser, on a 4 octet pour convertir a
//        @return
//        vecteur<int> b = nombre a converti en hexadécimal avec n digit
        int v;
        for(unsigned int i = 0; i < n; i++){
            v = int((double(a) / (pow(16, (n-1)-i))));
            a = a % int((pow(16, (n-1)-i)));
            b.push_back(v);
        }
     return b;
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
    vector<int> intToBin(const int conv[]){

        vector<int> vec(8);
        unsigned int d = hexToInt(conv, 1);

        bitset<8> b(d);
//        cout << "Le bitset" << ":" << b << endl;
        for (unsigned int j = 0; j < 8; j++) {
            vec[j] = b[(8-1)-j];
        }
        return vec;
    }


    /**
     * Plus tard comme CV::imgshow()
     */
    void show(){};
};
#endif //PARSER_MP_IMAGE_H

// H(eader)   : 1 seul, au début
// P(alette)  : 0 ou 1, Après H avant D
// D(onnées)  : >= 1 bloc à agrégé
// C(comment) : n'import où sauf en 1st

/*
 *                          Type(chunk)          Longueur           Contenu
 *                           1                      l
 *                           1                      4                  l
 *
 * Big Endian    2en, --> 2e1, 2e0
 * Little Endian 2e0, 2e1 --> 2en
 *
 *
 *  Contenu H
 *   W       H        pixel
 *   4       4          1
 *
 * Imposer que les nb de pixels soit mult de 8 (  très dur )
 * Imposer que la taille des données soit minimal. et les derniers bits sont à 0 ( un peu mieux )
 *
 *
 *
 *
 */