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

void Image::getHistogram(int values[]) const {
    
    for ( int tone = 0; tone < 256; tone++) {
        
        values[tone] = 0;
        for ( int x = 0; x < width(); x ++) {
            for ( int y = 0; y < height(); y++) {
                if (getPixel(x,y) == tone) values[tone]++;
            }
            
        }
    }
    
}




