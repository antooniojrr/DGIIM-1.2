/**
 * @file Byte.cpp
 * @author MP
 */
#include <iostream>
#include <string>
#include "MPTools.h"
#include "Byte.h"


using namespace std;



void Byte::onBit( int pos)
{
    
    Byte bit = 1;
    
    bit=bit.getValue() << pos;
    
    _data = _data|bit.getValue();
   
}


void Byte::offBit(int pos)
{
    
    Byte bit(1);
    
    bit = bit.getValue() << pos;
    bit = ~bit.getValue() ;
    
    _data = _data & bit.getValue();
    
}



bool Byte::getBit(int pos) const
{
    bool activado=true;
    if ( pos >= 0 && pos < NUM_BITS) {
        Byte bit(1);
        bit = bit.getValue() << pos;
        bit = bit.getValue() & _data;

        if (bit.getValue() == 0) 
            activado=false;
    }
    
    else cout <<"Error: posicion no valida" <<endl;
    
    
    return activado;
    
}


string Byte::to_string() const
{
    
    string cad_byte;
    
    for ( int i=NUM_BITS-1; i>=0; i-- ) {
        
        if (getBit(i)) 
            cad_byte += "1";
        else 
            cad_byte += "0";
        
    }
    
    return cad_byte;
    
}




void Byte::onByte()
{
    
    for ( int i=0; i<NUM_BITS ; i++ ){
        
        onBit(i);
        
    }
    
}


void Byte::offByte()
{
    
    for ( int i=0; i<NUM_BITS ; i++ ){
        
        offBit(i);
        
    }
    
}


void Byte::encodeByte(const bool v[])
{
  
    for ( int i=0; i<NUM_BITS; i++) {
        
        if (v[7-i]) 
            onBit(i);
        else 
            offBit(i);
        
    }

}


void Byte::decodeByte(bool v[]) const
{
    
    for ( int i=0; i<NUM_BITS; i++) {
        
        if (getBit(i)) 
            v[i] = true;
        else 
            v[i] = false;
        
    }
    
}


void Byte::decomposeByte(int posic[], int  &cuantos) const
{
    
    cuantos=0;
    
    for ( int i=0; i<NUM_BITS; i++) {
        
        if (getBit(i)) {
            
            posic[cuantos]=i;
            cuantos++;
            
        }
        
    }
    
}


void Byte::shiftRByte(int n) {
    
    _data = _data >> n;
    
}


void Byte::shiftLByte(int n) {
    
    _data = _data << n;
    
}


void Byte::mergeByte(Byte merge, int percentage){
    
    _data = (_data *(100-percentage)+merge.getValue()*percentage)/100;
    
}
