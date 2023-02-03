/**
 * @file Image.cpp
 * @note To be implemented by students 
 * @author MP-DGIM, MP-IADE, MP-II (grupo B)
 */
#include<iostream>
#include <fstream>
#include<cstring>
#include "MPTools.h"
#include "Byte.h"
#include "Image.h"
#include "Histogram.h"
using namespace std;

Image::Image() {
    
    _width = 0;
    _height = 0;
    
}

Image::Image(int width, int height) {
    if ( width * height <= IMAGE_MAX_SIZE) {
        _width = width;
        _height = height;

        for ( int x = 0; x < width; x++) {
            for ( int y = 0; y < height; y++)
                setPixel(x,y,0);
        }
        
    } else {
        
        cout << "Error: dimensiones maximas excedidas" << endl;
        _width=0;
        _height=0;
        
    }
}


int Image::height() const { return _height; }

int Image::width() const { return _width; }

void Image::setPixel(int x, int y, Byte v) {
    
    setPos(y*width() + x, v);
    
}

int Image::getPixel(int x, int y) const {
    
    return getPos((y*width()) + x);
    
}

void Image::setPos(int i, Byte v) {
    
    if ( i < width()*height() && i >= 0) {
        _data[i] = v;
    }
    else cout << "Error: posicion no valida" << endl;
    
}

int Image::getPos(int i) const {
    
    int valor = -1; //Devolverá menos uno cuando se le introduzca una
                    // posición incorrecta.
    
    if ( i < (width()*height()) && i >= 0) 
        
        valor = _data[i].getValue();
                
    else cout << "Error: posicion no valida" << endl;
    
    return valor;
    
}

void Image::flatten(Byte b) {
    
    for ( int x = 0; x < width(); x++) {
        
        for ( int y = 0; y < height(); y++) setPixel(x,y,b);
        
    }
}

void Image::showInWindow(string title) const {
    string tempname = "data"+SLASH+".hidden_" + title + ".pgm";

    mpSavePGMImage(tempname, (const unsigned char *)_data, _width, _height);
    mpDisplayImage(tempname, title);
    
}

string Image::inspect() const {
    string result = "";
    result = std::to_string(_width) + "x" + std::to_string(_height) + " " + std::to_string(mphash((const unsigned char*) _data, _height * _width));
    return result;
}

/**
     * @brief It opens a file that contains a PGM Image and reads the data to build
     * an image object. Modifier method. 
     * @param filename Name of the file. input parameter a c-string.
     * @return a code that means the following: 0 - Successful operation. 
     * 1 - Error opening the file
     * 2 - Error reading the data
     * 3 - The detected data does not follow the PGM technical description
     */
        
    int Image::readFromFile(const char filename[]){
        
        const string FORMAT1 = "P2";
        const string FORMAT2 = "P5";
        int value = IMAGE_DISK_OK;
        string comprobacion1;
        int valor_max;
        int alto, ancho;
    
        ifstream fentrada;
        
        string nombre = filename;
        int pixeles[IMAGE_MAX_SIZE];

        fentrada.open(nombre.c_str());

        if (fentrada) {

            fentrada >> comprobacion1;
            fentrada >> ancho;
            fentrada >> alto;
            fentrada >> valor_max;
            
            if ( (ancho * alto) > IMAGE_MAX_SIZE) {
                        
                value = IMAGE_TOO_LARGE;
                        
            }
            
            else {
            
                for ( int i = 0; i < (ancho*alto); i++) fentrada >> pixeles[i];

                if (fentrada) {

                    if (comprobacion1 != FORMAT1 && comprobacion1 != FORMAT2) {

                        value = IMAGE_ERROR_FORMAT;

                    }
                    
                    else {
             
                        _height = alto;
                        _width = ancho;
                        for ( int i = 0; i < ancho*alto; i++) 
                            setPos(i,pixeles[i]);
                    }

                }

                else value = IMAGE_ERROR_DATA;
            }
            fentrada.close();
        }
        
        else value = IMAGE_ERROR_OPEN;
        
        return value;
    }
    
    /**
     * @brief It writes the Image on disk, in PGM ascii format. Query method.
     * @param filename The name of the disk file which will contain the image. filename input parameter a c-string.
     * @return The same code that readFromFile()
     */
    int Image::saveToFile(const char filename[]) const {
        
        ofstream fsalida;
        
        const string TIPO_ARCHIVO = "P2";
        
        int value = IMAGE_DISK_OK;
        
        int alto = height();
        int ancho = width();
        const int MAX_VALOR = 255;
        string nombre = filename;
        
        fsalida.open(nombre.c_str());
        
        
        if (fsalida) {
            
            if (alto*ancho > IMAGE_MAX_SIZE)
                value = IMAGE_TOO_LARGE;
           
            else {
            
                fsalida << TIPO_ARCHIVO << endl;
                fsalida << ancho << " " << alto << endl;
                fsalida << MAX_VALOR << endl;

                for ( int i = 0; i < alto*ancho; i++ )

                    fsalida << getPos(i) << endl;


            }
            
            if (!fsalida) 
                value = IMAGE_ERROR_DATA;
            
            fsalida.close();
                    
        }
        
        else value = IMAGE_ERROR_OPEN;
        
        
        return value;
  
        
    }
    /**
     * @brief It calculates the histogram of the image, and returns it into an
     * instance of the class Histogram. Query method.
     * Deprecated    void getHistogram(int values[]), the previous can be removed
     
     */
    Histogram Image::getHistogram() const {
    
    Histogram my_histogram;
    int contador;
    
    for (int tone = 0; tone < 256; tone++) {
        contador = 0;
        for (int x = 0; x < width(); x ++) {
            for (int y = 0; y < height(); y++) {
                if (getPixel(x,y) == tone){
                    contador++;
                   
                }
            }  
        }
         my_histogram.setLevel(tone, contador);
    }
    
    return my_histogram;
}
    
    /**
     * @brief It takes the histogram of the image and depicts a new image with the
     * visualization of the histogram according to these rules 
     * ~~~~
     *   +-     +-----------------------------o 
     *   |      |                             | --- 1 pix white line
     *   |      |   *                         | -+
     *   |      |   **                        |  |
     *   |      |   **                        | 
     *   |      |   ***                   *   |   Normalized histogram
     *          |   ***                  **   |     150 pix
     *   h=160  |   ****    *            **   |  |
     *          |   *****  ***          ****  |  |
     *   |      |  ************************** | -+
     *   |      |-----------------------------|  1 pix  white line
     *   |      |-----------------------------| -+
     *   |      |                             |  8 pix Scale of gray levels
     *   +-     o-----------------------------+ -+
     *
     *          |                             |
     *          +--------- w  = 256 ----------+
     * ~~~~
     * Query method.
     * @return the built image
     */
    Image Image::depictsHistogram() const {
        
        int width = 256, height = 160, border = 1, foot_tone = 9, a = height -(border+foot_tone)-2;

        Image result(width, height);
        Histogram my_histogram = getHistogram();
        
        int max = my_histogram.getMaxLevel();
        
        result.flatten(255);

        for (int x = 0; x < width; x++) {

            int altura = height - ((my_histogram.getLevel(x)*150/
                                   max)+foot_tone+border);

            for ( int y = border; y < height-foot_tone - border; y++) {

                if ( y >= altura) result.setPixel(x,y,0);

            }
            
            result.setPixel(x,149,0);

            for (int y = 0; y < foot_tone; y++) { // the foot tone in the image
                
                result.setPixel(x, height-border-y, x);
            }

        }
        return result;
        
    }
    
   /**
     * @brief It segments the histogram by groups whose value is higher than the admitted tolerance and returns
     * an array of images, each of which corresponds to one of these objects. Query method
     * Given the following histogram, cut by the tolerance level. The method find 4 groups.
     * ~~~~
     *   +-     +-----------------------------o 
     *   |      |                             | --- 1 pix white line
     *   |      |   *                         | -+
     *   |      |   **                        |  |
     *   |      |   **                        | 
     *   |      |   ***                   *   |   Normalized histogram
     *          |   ***                  **   |     150 pix
     *   h=160  |   ****    *            **   |  |
     *          |   *****  ***          ****  |  |
     *   |      |  ******  ***   *     ****** | -+
     *   |      |-----------------------------|  tolerance level 
     *   |      |-----------------------------| -+
     *   |      |-----------------------------| -+                        
     *   +-     o-----------------------------+ -+
     *          |                             |
     *          +--------- w  = 256 ----------+
     * ~~~~
     * @param set The array of images. input/out param
     * @param nimages The number of images found. output param
     * @param maximages The max number of images to be found. input param
     * @param tolerance The tolerance admitted. input param
     */
    void Image::extractObjects(Image set [], int &nimages, int maximages, double tolerance) {
        
        Histogram my_histogram = getHistogram();
        int principio_g;
        int final_g;
        double freq_min = tolerance * my_histogram.getMaxLevel();
        int j = 0;
        nimages = 0;
        
        while ( j < 256 && nimages < maximages) {
            
            Image group(width(),height());
            
            
            while (my_histogram.getLevel(j) < freq_min && j < height()*width())
                j++;
            
            principio_g = j;
            
            while (my_histogram.getLevel(j) >= freq_min && j < height()*width())
                j++;
            
            final_g = j;
            
            for ( int i = 0; i < height()*width() ; i++) {
            
                if (getPos(i) < principio_g || getPos(i) > final_g)
                    
                    group.setPos(i,0);
                
                else group.setPos(i,getPos(i));
                
                
            }
            set[nimages] = group;
            nimages++;
            
        }
        
        
    }





