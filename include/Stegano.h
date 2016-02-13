#ifndef STEGANO_H
#define STEGANO_H

#include <string>
#include <Magick++.h>
#include <iostream>
#include <random>
#include <fstream>
#include <set>
#include "SteganoExceptions.h"
#include "SteganoStructs.h"

class Stegano
{
public:
    virtual ~Stegano();

    void loadPicture(const std::string &);
protected:
    Stegano();
    // TODO: make steganoImage private and create Getter
    Magick::Image steganoImage;

    void pushPixelBy(Pixel &, const int &);
    bool isFinishPixel(const Pixel &);
    unsigned int getRandomNumber(const unsigned int &, const unsigned int &);
    void incrementPixel(Pixel &);
    virtual bool isPixelEmpty(const Pixel &) = 0;
    unsigned int getFileStreamSizeInBytes(std::ifstream &);
    virtual unsigned char getDoneStateInPercent() = 0;

    double xResolution;
    double yResolution;
    double pixelAmount;
    std::string usedPixels;

    /** \brief This mod function is needed because c++ standard mod operator (%) doesn't work properly in a mathematical way on negative numbers.
     * The result of mod(-6, 4) would be 2, mod(9, 3) = 0, mod(8, -3) = -1, ...
     * \param &x const int the value that should be taken modulo
     * \param &m const int the modulo value
     * \return inline int result of x mod m
     */
    inline int mod(const int &x, const int &m)
    {
        return (x%m + m)%m;
    }

    /** \brief convert ImageMagick rgb values from default 16-bit (max. 65536) value to 8 bit value (max. 255)
     *
     * \param &rgb16Bit const unsignedint a rgb value in 16 bit
     * \return unsigned char a valid rgb value in range of 0 to 255 (including 0 and 255)
     */
    inline unsigned char convert16BitTo8BitRGB(const unsigned int &rgb16Bit)
    {
        return rgb16Bit / 257;
    }

    /** \brief calculate quadratic sondation of a given index; it is always lower than maxSize.
     * Collisions are possible
     * \param &index const unsignedint the index that should be sondated
     * \param &maxSize const unsignedint the maximum size of the return-value
     * \return int the result of the quadratic sondation
     */
    inline int_least64_t quadraticSondation(const unsigned int &index, const unsigned int &maxSize)
    {
        return (int_least64_t)std::pow(-1, index) * ((int_least64_t)std::pow(index, 2) % maxSize);
    }


private:
};

#endif // STEGANO_H
