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

#define DEBUG
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

    Image(const char* filename){
        ifstream f;
        string s;
        f.open(filename); // TODO:Ici spécifier les droits

//        Ouverture du fichier et chargement intégrale dans une chaine "s" puis fermeture
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
#ifdef DEBUG
        cout << "\"" << s << "\"" << s.length() << endl;
#endif
//        Récuperation du magic number et verification : si incorrect alors exit
        magic_number = string(s.substr(0, 8).c_str());
        cout << magic_number << endl;
        if(magic_number == "Mini-PNG"){

            // Création de blocs puis iteration dessus. La liste des blocs
            // parcourru est sauvegardée dans vector blocks
            unsigned int i = 8;
            Block b;
            b.type = s[8];
            while(i < s.length()) {

                // Si le bloc commence par H|C|D alors on procède comme la spec l'indique
                // Sinon on ne connait pas ce bloc alors ERROR
                if ((b.type == 'H')
                    | (b.type == 'C')
                    | (b.type == 'D')) {
                    collect_len_bloc(s, i, b);
                    collect_content_bloc(s, i, b);

                } else {
                    cout << "ERROR::FILE::Corrupted !" << endl;
                }
            }


        }else
        {
            cout << "ERROR::FILE::Structure not match !" << endl;
            exit(-1);
        }

        // On vérifie qu'on a au moins un block H dans la liste
        // Sinon Fichier invalide
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

    /**
     * Calcul l'entête "longueur du contenu" pour chaque bloc
     * (On s'assure dans la fonction_collect_content_bloc que sa valeur correspond
     * à la longueur réelle du contenu qui va être lu
     *
     * @param s   : chaine représentant le contenu du fichier
     * @param pos : la position courant du parser   (reference mutable ext)
     * @param b   : le bloc courant que l'on traite (reference mutable ext)
     */
    void collect_len_bloc(const string s, unsigned int& pos, Block& b){

        string t("Longueur du contenu ");

#ifdef DEBUG
        cout << "Working block " << b.type
             << " at pos:" << pos
             << "..." << endl;
        t = t + s[pos] + ": ";
        cout << t;
        for(int k = 1; k < 5; k++){
            cout  << hex << static_cast<int>(s[pos + k]);
        }
        cout << "Hex"<< endl;
#endif
        pos += 1;
        b.content_size = hexToInt(s, pos);
        pos += 4;
        b.total_len = 1 + 4 + b.content_size;
#ifdef DEBUG
        cout  << dec << t << setw(4) << setfill('0') << b.content_size <<"Int" << endl;
#endif
    }

    /**
     * Collecte le contenu du bloc courant, le stock dans l'attribut correspondant
     * S'assure que celui ci respecte la taille collectée par collect_len_bloc.
     *
     * Notes: Soit k octets de contenu lu. Si k != l alors:
     * -soit il y a un octet en moins
     * -soit il y a un octet en trop
     *
     * @param s   : chaine représentant le contenu du fichier
     * @param pos : la position courant du parser       (reference mutable ext)
     * @param b   : le bloc courant que l'on traite     (reference mutable ext)
     */
    void collect_content_bloc(const string& s, unsigned int& pos, Block& b){

        unsigned int l = b.content_size;
        for(unsigned int k = 0; k < l + 1; k++){
            unsigned int offset = pos + k;

            if((s[offset] == 'H')
               | (s[offset] == 'C')
               | (s[offset] == 'D')
               | (offset == s.length())){
                if(k != l){
                    cout << dec << "ERROR::BLOCK::CONTENT_INCOMPLETED !" << endl
                         << s[offset] << ' ' << offset << ' ' << pos << ' '
                         << static_cast<int>(s[offset-1])
                         << static_cast<int>(s[offset+1])<<endl ;
#ifdef DEBUG
                    string ss("Content of ");
                    ss += b.type + string(":");
                    cout << ss << "\"" << b.content << "\"" << endl;
#endif
                    exit(-1);
                }
                if(b.type == 'H'){
                    this->cols = hexToInt(b.content, 0);
                    this->rows = hexToInt(b.content, 4);
                    this->pixel_type = hexToInt(b.content, 8, 1);
                }
                blocks.push_back(b);

#ifdef DEBUG
                cout << "Current Block " << b.type << endl
                     << "total len:" << b.total_len << endl
                     << "content_size:" << b.content_size << endl
                     << "content:"
                     << "\""<< string(b.content) << "\"" << endl;
#endif

                if(offset < s.length()){
#ifdef DEBUG
                    cout << "Next bloc " << s[offset] << " at position " << pos + k << " of s" << endl;
#endif
                    Block next;
                    next.type = s[offset];
                    b = next;

                }
                pos += k;
            }else
            {
                if(offset < s.length()){
#ifdef DEBUG
                    cout << "Collecting content loop "
                         << pos << ' ' << k
                         << " s[i+k]=" << hex << static_cast<int>(s[offset]) << dec << endl;
#endif
                    if(b.type == 'D'){
                        this->data.push_back(s[offset]);
                    }
                    b.content.push_back(s[offset]);
                }

            }

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

        for(char e : data)
            cout << setw(4) << setfill('0') << hex
                 << static_cast<int>(e) << endl;

        vector<string> bitmap = hexToBin(this->data, 0, 4);
        for(unsigned int i = 0; i < rows; i++) {
            for(unsigned int j = 0; j < cols; j++) {
                cout << ' ' << bitmap[i][j] << ' ';
            }
            cout << endl;
        }

    }

    /**
     * Convertion hexadécimal vers décimal integer
     * @param s
     * @param pos
     * @param stride
     * @return
     */
    unsigned int hexToInt(const string &s, const int pos, unsigned int stride = 4){
        unsigned int l = 0;
        for(unsigned int j = 0; j < stride; j++){
            l += static_cast<int>(s[pos+j]) * pow(16, ((stride-1)-j));
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
     */
    vector<string> hexToBin(const string s, unsigned int pos, unsigned int stride = 4){

        vector<string> bin;
        unsigned int k = s.length();
        unsigned int l = hexToInt(s, 0, k);
        int a;

        a = l;
        for(unsigned int i = 0; i < cols; i++) {
            string b;
            for (unsigned int j = 0; j < rows; j++) {
                a = a - int(pow(2, double(stride-j)));
                if(a >= 0){
                    b.push_back(' ');
                } else
                {
                    b.push_back('x');
                }

            }
            bin.push_back(b);
        }
        return bin;
    }


    /**
     * Plus tard comme CV::imgshow()
     */
    void show(){};
};
#endif //PARSER_MP_IMAGE_H
