/**
 * @file main.cpp
 * @author MP
 */
#include <iostream>
#include <cstring>
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


int main() {
    #define MAXCOLLECTION 10
    Image im_input,im_histogram;
    Image im_collection [MAXCOLLECTION];
    Histogram histogram;
    int nimages=0, result;
    string input;
    
    input="./data/burgerking.pgm";

		// Leer la imagen de entrada y comprobar los posibles errores
    int value = im_input.readFromFile(input.c_str());
    
    if (value == Image::IMAGE_ERROR_DATA) 
        cout << "ERROR: datos corruptos" << endl;
    else if (value == Image::IMAGE_TOO_LARGE)
        cout << "ERROR: dimensiones máximas excedidas" << endl;
    else if (value == Image::IMAGE_ERROR_OPEN)
        cout << "ERROR: no se pudo abrir la imagen" << endl;
    else if (value == Image::IMAGE_ERROR_FORMAT)
        cout << "ERROR: línea de formato corrupta" << endl;
    
		// Mostrar la magen de entrada en pantalla
    im_input.showInWindow("input");
    CVAL << input << endl;
    INSPECT(im_input);
    
   	// Construir el histograma de la imagen de entrada y mostrarlo por pantalla
    histogram = im_input.getHistogram();
    im_histogram = im_input.depictsHistogram();
    im_histogram.showInWindow("Histogram");
    INSPECT(im_histogram);
    
    	// Extraer los objetos en el vector de imágenes
    im_input.extractObjects(im_collection, nimages, MAXCOLLECTION);
    string name;
    for (int i=0; i<nimages; i++) {
	im_collection[i].showInWindow("objeto");
        INSPECT(im_collection[i]);
        name="./data/"+std::to_string(i)+".pgm";
        im_collection[i].saveToFile(name.c_str());
    }
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
            CVAL << endl << "The image is too large and does not fit into memory" << errordata << endl;
            break;
    }
    std::exit(1);
}
