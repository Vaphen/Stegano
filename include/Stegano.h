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
        std::string getRGBString(const RGB &);
        unsigned int getRandomNumber(const unsigned int &, const unsigned int &);
        void incrementPixel(Pixel &);
        unsigned char convert16BitTo8BitRGB(const unsigned int &);
        int_least64_t quadraticSondation(const unsigned int &, const unsigned int &);
        virtual bool isPixelEmpty(const Pixel &) = 0;
        inline int mod(const int &x, const int &m);
        unsigned int getFileStreamSizeInBytes(std::ifstream &);
        virtual unsigned char getDoneStateInPercent() = 0;

        double xResolution;
        double yResolution;
        std::string usedPixels;
    private:
};

#endif // STEGANO_H
