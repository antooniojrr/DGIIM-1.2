/**
 * @file main.cpp
 * @author MP
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include "MPTools.h"
#include "Byte.h"
#include "Image.h"
#include "Histogram.h"

using namespace std;

/**
 * @brief It waits until the key [INTRO] [RETURN] is pressed
 */
void pressReturnToContinue();

/**
 * @brief It breaks the program due to some error and shows a message
 * @param errorcode The type of error as declared in Image.h
 * @param errordata A message trying to describe the error
 */
void errorBreak(int errorcode, const string & errordata);

int main( int argc, char** argv) {
    
    string input = "";
    string copy = "";
    string output = "";
    string cod_text = "";
    string cod_im = "";
    int zoom = 0;
    int plano = 0;
    bool copy_introd = false;
    bool output_introd = false;
    bool input_introd = false;
    bool zoom_introd = false;
    bool plano_introd = false;
    bool ht_introd = false;
    bool st_introd = false;
    bool hi_introd = false;
    bool si_introd = false;
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-i" && !input_introd){
            input = argv[i + 1];
            input_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-c" && !copy_introd){
            copy = argv[i+1];
            copy_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-o" && !output_introd){
            output = argv[i+1];
            output_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-z" && !zoom_introd){
            zoom = stoi(argv[i+1]);
            zoom_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-p" && !plano_introd){
            plano = stoi(argv[i+1]);
            plano_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-ht" && !ht_introd){
            cod_text = argv[i+1];
            ht_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-hi" && !hi_introd){
            cod_im = argv[i+1];
            hi_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-st" && !st_introd){
            st_introd = true;
        }
    }
    
    for (int i = 0; i < argc; i++){
        string codigo = argv[i];
        if(codigo == "-si" && !si_introd){
            si_introd = true;
        }
    }
    
    Image im_input, im_output, im_copyfrom, im_bin, im_cut, im_histogram, im_collection[10], im_aux;
    int res, n, w, h;
    res = im_input.readFromFile(input.c_str());
    if (res != Image::IMAGE_DISK_OK) {
        errorBreak(res, input);
    }
    
    //im_input.showInWindow("input");
//        im_input.depictsHistogram().showInWindow("histoinput");
//        im_aux = im_input.threshold(128);
//        im_aux.showInWindow("input128");
//        im_aux.depictsHistogram().showInWindow("histo128");
//        
//        im_aux=im_input.threshold();
//        im_aux.showInWindow("inputbalance");
//        im_aux.depictsHistogram().showInWindow("histobalance");

    //INSPECT(im_input);
    
    
    if (zoom != 0) {
        
        if ( zoom == 1) {
            im_input=im_input.zoomIn();
            cout << "...Zooming in" << endl;
        }
        else {
            im_input=im_input.zoomOut();
            cout << "...Zooming out" << endl;
        }
        
        //im_input.showInWindow("input_zoomed");
        //INSPECT(im_input);
    
    }
    
   
    
    /*if (copy.length() > 0) {
        res = im_copyfrom.readFromFile(copy.c_str());
        if (res != Image::IMAGE_DISK_OK) {
            errorBreak(res, input);
        }
        im_copyfrom.showInWindow("copyfrom");
        im_copyfrom.depictsHistogram().showInWindow("copyhisto");
        INSPECT(im_copyfrom);
        w = im_copyfrom.width() + 5;
        h = im_copyfrom.height() + 5;
        im_bin = im_copyfrom.threshold();
        im_bin.showInWindow("threshold");
        INSPECT(im_bin);
        int x = 0;
        im_input.pasteArea(x, 0, im_copyfrom);
        im_copyfrom.extractObjects(im_collection, n, 3);
        im_cut = im_collection[0];
        im_collection[0].showInWindow("segmented");
        INSPECT(im_collection[0]);
        x = x + w;
        im_input.pasteArea(x, 0, im_cut);
        im_input.pasteArea(x, h, im_bin);
        x = x + w;
        im_input.pasteArea(x, 0, im_cut, 64);
        im_input.pasteArea(x, h, im_bin, 64);
        x = x + w;
        im_input.pasteArea(x, 0, im_cut, 64, 50);
        im_input.pasteArea(x, h, im_bin, 64, 50);
    }*/
    const int MAX=20000;
    cout << "Carrier: " << input << endl << endl;
    if (ht_introd) {
        ifstream texto(cod_text.c_str());
        char texto_st[MAX];

        texto >> texto_st;
            
        texto.close();
        bool ok = im_input.hideText(texto_st,plano);
        
    }
    
    if (st_introd) {
        char cadena[MAX];
        bool ok = im_input.showText(cadena,MAX,plano);
        if (ok) cout << cadena;
        if (output_introd) {
            ofstream texto(output);
            texto << cadena;
            texto.close();
            output_introd = false;
        }
    }
    
    if (hi_introd) {
        Image codif;
        codif.readFromFile(cod_im.c_str());
        bool ok = im_input.hideImage(plano,codif);
        
    }
    
    if (si_introd) {
        Image decod;
        bool ok = im_input.showImage(plano,decod);
        //decod.showInWindow("hide_im");
        //INSPECT(decod);
        if (output_introd) {
            decod.saveToFile(output.c_str());
            output_introd = false;
        }
    }
    
    im_output = im_input;
    if (output_introd) {
        im_output.saveToFile(output.c_str());
    }
    //im_output.showInWindow("output");
    //INSPECT(im_output);
    pressReturnToContinue();
    return 0;
}
        

void pressReturnToContinue() {
    char aux[10];
    cout << "Press [RETURN] to continue ...";
    cin.getline(aux, 1, '\n');
}

void errorBreak(int errorcode, const string & errordata) {
    switch (errorcode) {
        case Image::IMAGE_ERROR_OPEN:
            CVAL << endl << "Error opening file " << errordata << endl;
            break;
        case Image::IMAGE_ERROR_DATA:
            CVAL << endl << "Data error in file " << errordata << endl;
            break;
        case Image::IMAGE_ERROR_FORMAT:
            CVAL << endl << "Unrecognized format in file " << errordata << endl;
            break;
        case Image::IMAGE_TOO_LARGE:
            CVAL << endl << "Error: " << errordata << " too big" << endl;
            break;        
    }
    std::exit(1);
}
