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
        _data = new Byte* [height];
        _data[0] = new Byte[height*width];
        for ( int i = 1; i < height; i++)
            _data[i]=_data[i-1]+width;

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
    
    _data[y][x] = v;
    
}

int Image::getPixel(int x, int y) const {
    
    int valor = -1; //Devolverá menos uno cuando se le introduzca una
                    // posición incorrecta.
    
    if ( x>=0 && x<width() && y>=0 && y<height())
        valor = _data[y][x].getValue();
    
    return valor;
    
}

void Image::setPos(int i, Byte v) {
    
    setPixel(i%width(),(int)i/width(), v);
    
}

int Image::getPos(int i) const {
    
    int valor = getPixel(i%width(),(int)i/width());
        
    
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
    
    if (_data != nullptr) {
        delete[] _data[0];
        delete[] _data;
        _data = nullptr;
        _height = 0;
        _width = 0;
    }
    
}

void Image::copy( const Image &rhs ) {
    
    setSize(rhs.height(),rhs.width());
    
    for (int i = 0; i < height()*width(); i++) 
        
        setPos(i,rhs.getPos(i));
       
}

void Image::setSize( int newheight, int newwidth) {
    
    if ( newwidth * newheight <= IMAGE_MAX_SIZE) {
        clear();
    
        _width = newwidth;
        _height = newheight;
        _data = new Byte* [newheight];
        _data[0] = new Byte[newheight*newwidth];
        for ( int i = 1; i < newheight; i++)
            _data[i]=_data[i-1]+newwidth;

        for ( int x = 0; x < newwidth; x++) {
            for ( int y = 0; y < newheight; y++)
                setPixel(x,y,0);
        }
    }
}

Image::~Image() { clear(); }

void Image::operator =(const Image& rhs) { copy(rhs); }

/**
 * @brief It seeks for a hidden image in plane k. In order to do that, it
 * must first check that the first letters match the cookieImage constant. 
 * If it does not, it must return false since it is not possible. Otherwise,
 * the next bytes contain the width and height of the image, values in [0..255]. Finally the following 
 * width*height bytes encode the content of the hidden image, returned as a result. Query method
 * @param k The plane. input param
 * @param result The found image. output param
 * @return True when a valid image is found, false otherwise
 */
bool Image::showImage(int k, Image &result) const {
    
    bool oculto = true;
    cout << "...Seeking image in plane " << k;
    string IMG = Byte((int)'I').to_string()+Byte((int)'M').to_string();
    IMG += Byte((int)'G').to_string();
    
    int i = 0;
    while ( oculto && i < 24) {
        Byte pixel(getPos(i));
        if ((pixel.getBit(k)&&IMG.at(i)=='0') || (!pixel.getBit(k)&&IMG.at(i)=='1'))
            oculto=false;
        i++;
        
    }
    if (oculto) {
        // Leemos la altura
        int altura=0;
        for ( int i = 24; i < 32; i++) {
            Byte pixel(getPos(i));
            if (pixel.getBit(k)) altura += pow(2,31-i);
        }
        
        // Leemos el ancho
        int ancho=0;
        for ( int i = 32; i < 40; i++) {
            Byte pixel(getPos(i));
            if (pixel.getBit(k)) ancho += pow(2,39-i);
        }
        cout << "...Seeking image of " << altura << "x" <<ancho <<" pixels" <<endl;
        if ((altura*ancho+5)*8 <= width()*height()) {
            Image codificada(altura,ancho);
            for (int pixel = 0;pixel<altura*ancho&&pixel*8<=width()*height();pixel++){
                int valor=0;
                for ( int i = 0; i < 8; i++) 
                    if((int)Byte(getPos(i+((pixel+5)*8))).getBit(k)) valor+=pow(2,7-i);

                codificada.setPos(pixel,valor); 
            }
        
        result = codificada;
        }
        else oculto=false;
    }
    if (oculto) cout << "OK" << endl << endl;
    else cout << "Error: impossible to get an image" << endl << endl;
    return oculto;
}

    
/**
 * @brief It hides the image @a from by encoding the sequence of values of its pixels
 * into the bits of plane @a k. In order to do that, it first hides the cookieImage constant
 * to identify this plane as containing an image. Then it hides the width and the height. Then
 * all the bytes from left to right and top down. The self image is modified. Modifier method.
 * @param k The plane. input param
 * @param from The image to hide. input param
 * @return  It return true if the process is successful, false otherwise. 
 */

bool Image::hideImage (int k, const Image &from){
    
    bool todo_ok = true;
    cout << "...Trying to hide image in plane " <<k << endl;
    
    int n = (from.width()*from.height() + 5)*8;
    
    if (width() * height() > n && k >= 0 && k < 8 && from.width() < 256
        && from.height() < 256){
        
        Byte I = 'I';
        Byte M = 'M';
        Byte G = 'G';
        
        
        for (int i = 0; i < 8; i++){ 
            Byte my_byte (getPos(i));
            if (I.getBit(7-i)) my_byte.onBit(k);
            else my_byte.offBit(k);
            
            setPos(i,my_byte);

        }
        
        for (int i = 0; i < 8; i++){ 
            Byte my_byte (getPos(i+8));
            if (M.getBit(7-i)) my_byte.onBit(k);
            else my_byte.offBit(k);
            
            setPos(i+8,my_byte);
        }
        
        for (int i = 0; i < 8; i++){ 
            Byte my_byte (getPos(i+16));
            if (G.getBit(7-i)) my_byte.onBit(k);
            else my_byte.offBit(k);
            
            setPos(i+16,my_byte);
        }
        
        Byte ancho(from.width());
        
        for (int i = 0; i < 8; i++){
            Byte my_byte (getPos(i+24));
            if (ancho.getBit(7-i)) my_byte.onBit(k);
            else my_byte.offBit(k);
            
            setPos(i+24,my_byte);
        }
        
        Byte alto (from.height());
        
        for (int i = 0; i < 8; i++){
            Byte my_byte (getPos(i+32));
            if (alto.getBit(7-i)) my_byte.onBit(k);
            else my_byte.offBit(k);
            
            setPos(i+32,my_byte);
        }
        
        for (int i = 5; i < (from.width()*from.height())+5; i++){
            Byte pixel (from.getPos(i-5));
            for (int j = 0; j < 8; j++) {
                Byte my_byte(getPos(j+i*8));
                if (pixel.getBit(7-j)) 
                    my_byte.onBit(k);
                else my_byte.offBit(k);
                
                setPos(j+i*8,my_byte);
            }
        }
        
    }
    else 
        todo_ok = false;
    if (todo_ok) cout << "OK" << endl << endl;
    else cout << "Error: failed to hide the image" << endl << endl;
    
    return todo_ok;
}

/**
 * @brief Steganography process.
 * It hides the cstring given within the k-plane especified. If no
 * plane is specified, the 0-plane is considered. If the cstring is too
 * large given the size of the original image, it must return false. Otherwise
 * the image hides the cstring and it returns true. In order to do that, it first
 * hides the cookieText constant, then the content of the cstring and finally,
 * the \0 delimiter converted in 00000000 encoded into the image. Modifier method.
 * @param the cstring to hide. input param
 * @param k the k-plane. input param
 * @return true if it has been possible, false otherwise
*/

bool Image::hideText(const char source [], int k) {

    bool todo_ok = true;
    cout << "...Trying to hide text in plane " <<k << endl;
    int longitud = 0;

    while (source[longitud] != '\0') longitud++;

    if ( (longitud+4)*8 > width()*height() ) todo_ok=false;

    else if ( k < 0 || k > 7 ) todo_ok = false;

    else {
        
        char *cad = new char[longitud+4];
        cad[0]=cad[2]='T';
        cad[1]='X';
        for ( int i = 3; i <= longitud+3; i++) cad[i] = source[i-3];
        
        for ( int i = 0; i <= longitud+3; i++) {
            Byte caract((int)cad[i]);
            int empieza = i*8;
            for ( int j = 0; j < 8; j++) {
                Byte pixel(getPos(j+empieza));
                if (caract.getBit(7-j)) pixel.onBit(k);
                else pixel.offBit(k);
                setPos(j+empieza,pixel);
                
            }
         delete[] cad;  
       }

    }
    if (todo_ok) cout << "OK" << endl << endl;
    else cout << "Error: failed to hide the image" << endl << endl;
    return todo_ok;

}

/**
 * @brief It checks that the k-plane contains a cstring if the first bytes 
 * of the image contains the cookieText constant. Afterwards, all the bytes
 * encided in plane k are extracted and the final one must be the \0 delimiter
 * @param hidden cstring to return the hidden string, if any
 * @param max size of the cstring, without including the \0 delimiter
 * @param k The plane
 * @return True if success, false otherwise.
 */
bool Image::showText(char hidden [], int max, int k) const {
    
    bool oculto = true;
    cout << "...Seeking text in plane " << k << endl;
    string TXT = Byte((int)'T').to_string()+Byte((int)'X').to_string();
    TXT += Byte((int)'T').to_string();
    
    int i = 0;
    while ( oculto && i < 24) {
        Byte pixel(getPos(i));
        if ((pixel.getBit(k)&&TXT.at(i)=='0') || (!pixel.getBit(k)&&TXT.at(i)=='1'))
            oculto=false;
        i++;
    }
    if (oculto) {
        
        // LECTURA ADELANTADA
        int caract=0;
        for ( int i = 0; i < 8; i++)
            
            if((int)Byte(getPos(i+24)).getBit(k)) caract+=pow(2,7-i);

        hidden[0]=caract;
        
        for (int pos = 1; pos < max && hidden[pos-1]!='\0';pos++) {
            int caracter=0;
            for ( int i = 0; i < 8; i++) 
                if((int)Byte(getPos(i+((pos+3)*8))).getBit(k)) caracter+=pow(2,7-i);
                
            hidden[pos]=caracter; 
        }
        
    }
    if (oculto) cout << "OK" << endl << endl;
    else cout << "Error: impossible to get a text" << endl << endl;
    return oculto;
}