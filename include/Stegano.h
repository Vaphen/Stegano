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
        Stegano();
        virtual ~Stegano();

        void loadPicture(const std::string &);
    protected:
        // TODO: make steganoImage private and create Getter
        Magick::Image steganoImage;

        void pushPixelBy(Pixel &, const int &);
        bool isFinishPixel(const Pixel &);
        std::string getRGBString(const RGB &);
        unsigned int getRandomNumber(const unsigned int &, const unsigned int &);
        void incrementPixel(Pixel &);
        unsigned char convert16BitTo8BitRGB(const unsigned int &);
        int_least64_t quadraticSondation(const unsigned int &, const unsigned int &);
        bool isPixelEmpty(const Pixel &);
        inline int mod(const int &x, const int &m);
        unsigned int getFileStreamSizeInBytes(std::ifstream &);

        double xResolution;
        double yResolution;
    private:
};

#endif // STEGANO_H
