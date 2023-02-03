/**
 * @file Image.cpp
 * @note To be implemented by students 
 * @author MP-DGIM, MP-IADE, MP-II (grupo B)
 */
#include<iostream>
#include <fstream>
#include<cstring>
#include<cmath>
#include "MPTools.h"
#include "Byte.h"
#include "Image.h"
#include "Histogram.h"

using namespace std;

Image::Image() {
    
    _width = 0;
    _height = 0;
    _data = nullptr;
    
}

Image::Image(int width, int height) {
    if ( width * height <= IMAGE_MAX_SIZE) {
        _width = width;
        _height = height;
        _data = new Byte[width*height];

        for ( int x = 0; x < width; x++) {
            for ( int y = 0; y < height; y++)
                setPixel(x,y,0);
        }
        
    } 
}

Image::Image(const Image & from) {
    
    copy(from);
   
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
    
    if ( i < width()*height() && i >= 0) 
        _data[i] = v;
    
    
}

int Image::getPos(int i) const {
    
    int valor = -1; //Devolverá menos uno cuando se le introduzca una
                    // posición incorrecta.
    
    if ( i < (width()*height()) && i >= 0) 
        
        valor = _data[i].getValue();
        
    
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
        int pixel;

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
               
                if (comprobacion1 != FORMAT1 && comprobacion1 != FORMAT2) {

                    value = IMAGE_ERROR_FORMAT;

                }

                else {

                    setSize(alto,ancho);

                    for ( int i = 0; i < (ancho*alto); i++) {
                        fentrada >> pixel;
                        setPos(i,pixel);
                    }
                    
                    if (!fentrada) value = IMAGE_ERROR_DATA;
                  
                }
                
                fentrada.close();
            }
            
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
    void Image::extractObjects(Image set [], int &nimages, int maximages, double tolerance) const{

        Histogram my_histogram = getHistogram();

        double freq_min = (my_histogram.getMaxLevel() * tolerance);
        bool tolerancia = true;
        nimages = 0;

        vector <int> posiciones_iniciales; 
        vector <int> posiciones_finales;

        int i = Histogram::HISTOGRAM_LEVELS - 1;

        while (nimages <= maximages && i >= 0){
            if (tolerancia) {

                if (my_histogram.getLevel(i) >= freq_min){
                    posiciones_finales.push_back(i);
                    tolerancia = false;

                    if (i == 0){
                        posiciones_iniciales.push_back(i);
                        nimages++;
                    }
                }  
            }

            else {

                if (my_histogram.getLevel(i) < freq_min || i == 0){

                    posiciones_iniciales.push_back(i);
                    nimages++;
                    tolerancia = true;
                }
            }

            i--;
        }

        for (int i = 0; i < nimages; i++){

            Image my_image(width(), height());

            for (int j = 0; j < (width()*height()); j++) {

                if(getPos(j) >= posiciones_iniciales.at(i) && getPos(j) <= posiciones_finales.at(i)){
                    my_image.setPos(j, getPos(j));
                }
            }

            set[i] = my_image;
        }  
    }
        
        
    
    /**
* It returns a binarization of the original image. All pixels  strictly greater than the value @a t
* are set to 11111111 and the others to 00000000. Query method.
* @param t The threshold. This value must be within [0,255]. In the case 
* that the threshold is not within these bounds, an automatic threshold is chosen,
* that is, the first level that leaves, at least the half of points less that
* or equal to it. Input parameter, default value is set to -1.
* @return A copy of the original image
*/
Image Image::threshold(int t) const{
    Image my_image(width(), height());
    Histogram my_histogram = getHistogram();
    
    if ( t < 0 || t > 255 ) t = my_histogram.getBalancedLevel();
    
    
    for (int i = 0; i < width()*height(); i++){
        if (getPos(i) <= t)
            my_image.setPos(i, 0);
        else 
            my_image.setPos(i, 255);
    }
  
    
       
    return my_image;
}

/**
* @brief It returns a subimage of the original image given by the parameters.
* If the parameters exceed  the dimensions of the original image, then 
* the cut image should not exceed those limits. Query method.
* @param x x-coordinate of the top left corner.
* @param y y-coordinate of the topt left corner
* @param w width of the subimage
* @param h height of the subimage. x,y,w, are input parameters.
* @return The subimage cut
*/
Image Image::copyArea(int x, int y, int w, int h) const{
    
    
    if (x+w > width())
        
        w = width() - x;
    
    if (y+h > height())
        
        h = height() - y;
    
    if ( x < 0 ) {
        
        w = w + x;
        x = 0;
        
    }
    
    if ( y < 0 ) {
        
        h = h + y;
        y = 0;
        
    }
    
    Image area(w,h);
    
    for ( int i = 0; i < w; i++) {
        
        for ( int j = 0; j < h; j++) 
            
            area.setPixel(i,j,getPixel(x+i,y+j));
        
    }
    
    
    return area;
}

/**
* @brief It pastes a secondary image into the original image, at the given position,
* into the same original image, which results modified. 
* This operation should maintain the limits of the original image
* @param x x-coordinate of the top left corner. input param
* @param y y-coordinate of the topt left corner. input param
* @param from The second image. input param
* @param toneup value. input param
* @param merge value for the combination of the 2 images. input param
*/
void Image::pasteArea(int x, int y, const Image& from, int toneup, int merge){
    
    for(int i = x; i < x+from.width() && i < width(); i++){
        for (int j = y; j < y+from.height() && j < height(); j++){
            
            if ( from.getPixel(i-x,j-y) > toneup) {
                
                int pixel = (merge*from.getPixel(i-x,j-y) + (100-merge)*getPixel(i,j))/100;

                setPixel(i, j, pixel);
                
            }
        }
    }
}

Image Image::zoomIn() const{
    Image zoom (_width*2, _height*2); 
    
    zoom.flatten(0);
    
    for (int i = 0; i < zoom.width(); i++){
        for (int j = 0; j < zoom.height(); j++){
            if (i%2 == 0 && j%2 == 0)
                zoom.setPixel(i, j, (getPixel(i/2,j/2)));
            else
                zoom.setPixel(i, j, copyArea(i/2 - 1, j/2 - 1, 3, 3).
                              getHistogram().getBalancedLevel());
        }
    }
    
    return zoom;
}

    /**
     * @brief Ti zooms the image out. Query method
     * @return The zoomed image
     */
Image Image::zoomOut() const{
    Image zoom (_width/2, _height/2); 
    
    zoom.flatten(0);
    
    for (int i = 0; i < zoom.width(); i++){
        for (int j = 0; j < zoom.height(); j++){
            zoom.setPixel(i, j, copyArea(i*2 - 1, j*2 - 1, 3, 3).
                          getHistogram().getBalancedLevel());
        }
    }
    
    return zoom;
}

void Image::clear() {
    
    delete[] _data;
    _data = nullptr;
    _height = 0;
    _width = 0;
    
}

void Image::copy( const Image &rhs ) {
    
    setSize(rhs.height(),rhs.width());
    
    for (int i = 0; i < height()*width(); i++) 
        
        setPos(i,rhs.getPos(i));
       
}

void Image::setSize( int newheight, int newwidth) {
    
    clear();
    _height = newheight;
    _width = newwidth;
    _data = new Byte[newheight*newwidth];
    
}

Image::~Image() { clear(); }

void Image::operator =(const Image& rhs) { copy(rhs); }