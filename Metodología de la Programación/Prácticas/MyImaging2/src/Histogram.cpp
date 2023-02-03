/**
 * @file Histogram.cpp
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

const double Histogram::HISTOGRAM_TOLERANCE = 0.01;
/**
 * @brief It builds an empty. Modifier.
 */
Histogram::Histogram() {
    
    for ( int i = 0; i < size(); i++)
        
        _data[i] = 0;
    
}

/**
 * @brief It returns the number of levels in the histogram
 * @return The number of levels
 */
int Histogram::size() const { return HISTOGRAM_LEVELS; }
/**
 * @brief Sets the whole histogram to 0. Modifier.
 */
void Histogram::clear() { for ( int i = 0; i < size(); i++) _data[i] = 0; }
/**
 * @brief It returns the value associated to the level indicated
 * @param level The level indicated
 * @return The value associated to the level
 */
int Histogram::getLevel(Byte level) const { return _data[level.getValue()]; }
/**
 * @brief It sets the value associated to the level. Modifier.
 * @param level The level
 * @param npixeles The new value
 */
void Histogram::setLevel(Byte level, int npixeles) {
    
    _data[level.getValue()] = npixeles;
    
}
/**
 * @brief It returns the maximum value stored
 * @return The max of the levels
 */
int Histogram::getMaxLevel() const {
    
    int max = 0;
    for ( int i = 0; i < size() ; i++)
        
        if ( _data[i] > max) max = _data[i];
    
    return max;
}
/**
 * @brief it returns the average value stored (divisor the length of vector)
 * @return The average level
 */
int Histogram::getAverageLevel() const {
    
    int suma = 0;
    for ( int i = 0; i < size(); i ++) suma += _data[i];
    
    return suma/size();       
}
/**
 * It returns a balance level, that is, the level that leaves half of the points
 * underneath or equal to it (math concept of median).
 * ex1. _data[4] = {5,5,0,10};  half -> 10
 * result 1
 * ex2. _data[4] = {10,5,5,0};  half -> 10
 * result 0
 * @return The point of balance of the histogram
 */
int Histogram::getBalancedLevel() const {
    
    int suma = 0;
    
    for ( int i = 0; i < size(); i++) suma += _data[i];
    
    int mitad = suma/2;
    int mediana = 0;
    suma = _data[mediana];
    
    while (suma < mitad) { mediana ++; suma += _data[mediana]; }
    
    return mediana;
       
}
/**
 * @brief It returns a unique hash code for every object so that they might be compared
 * @return The hash code as an string
 */

string Histogram::inspect() const{
    std::string result = "";
    for (int i = 0; i < HISTOGRAM_LEVELS; i++) {
        result += to_string(_data[i]);
    }
    result = std::to_string(mphash((unsigned char*)result.c_str(), result.length()));
    result = std::to_string(HISTOGRAM_LEVELS) + " " + result;
    
    return result;
}




