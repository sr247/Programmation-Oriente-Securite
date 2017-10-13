#include <iostream>
#include "Image.h"
#include <bitset>

using namespace std;

static int create(const char* filename){
    vector<Block> blist;
    vector<unsigned char> contenu;
    contenu.push_back((unsigned char)(8));
    contenu.push_back((unsigned char)(9));
    contenu.push_back((unsigned char)(0));

    blist.push_back(Block('H', 9, contenu));

    vector<string> bm{"00111110","01000000",
                      "10000000","01000000",
                      "00111100","00000010",
                      "00000001","00000010",
                      "01111100"};


    contenu.clear();

    for(string& s : bm){
        bitset<8> b(s);
        contenu.push_back((unsigned char)(bitset_to_long(b)));
    }

    blist.push_back(Block('D', 9, contenu, bm));

    int res = EXIT_SUCCESS;
    ofstream f;

    if (remove(filename) != 0) {
        perror("remove");
        exit(EXIT_FAILURE);
    }

    f.open(filename, ios::binary);
    if (!f.is_open()) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    f.write("Mini-PNG", sizeof("Mini-PNG")/sizeof(char));
//    cout << blist[0].type << endl;
//    for(char e : blist[0].content)
//        cout << e;

//    cout << endl << blist[1].type << endl;

    for(Block b : blist) {
        cout << "Ecriture bloc[" << b.type << ' ' << int(b.type) << "] -> file: " << string(filename) << endl;
        f.put(b.type);

        vector<int> content_size_en_hexa = Image::intToHex(b.content_size, 4);
        cout << "Sa taille: " << b.content_size << ' '
             << content_size_en_hexa[0]
             << content_size_en_hexa[1]
             << content_size_en_hexa[2]
             << content_size_en_hexa[3] << endl;

        for(int d : content_size_en_hexa){
            f.put(char(d));
        }

        for (unsigned int i : b.content) {
            f.put(char(i));
        }
    }
    f.close();

    return res;
}

int main(int argv, char ** args) {

    if(argv > 2){
        cout << "Too many arguments" << endl;
    }
    else if(!args[1]){
        cout << "Arg missing" << endl;
    } else {
// Regex pour test si le fichier est bien .mp
//        Image img(args[1]);
//        img.to_string();
//        img.show_comment();
//        img.show_ascii();
    }

    create("myFirstLetter.mp");
    Image My("myFirstLetter.mp");
    My.show_ascii();
    return 0;
}

// Envoyer à l'adresse : ol.pos2017@picty.org
// Ce que le parser fait:
//      -Les fichiers acceptés par le parser sont tout les fichiers prénommé *.mp
//      -Tout fichiers qui possèdent au moins un bloc header est accepté, le dernier bloc
//       étant traité comme le header valide (bien qu'on devrait déterminer que ce fichier
//       ne peut pas etre valide)
//      -Tout fichiers dont la dimension est > 0 implique que la taille des données est égale
//        à (L x H )/8 à l'arrondie supérieure.
//      -Si la taille des données annoncée par le champs longueur des données n'est pas respecté
//       alors on refuse ce fichier
//
