#ifndef STEGANOUNHIDE_H
#define STEGANOUNHIDE_H

#include <string>
#include <Magick++.h>
#include <iostream>
#include <sstream>
#include "SteganoExceptions.h"
#include "SteganoStructs.h"
#include "Stegano.h"
#include "PrivateChunk.h"

class SteganoUnhide : public Stegano
{
    public:
        SteganoUnhide();
        virtual ~SteganoUnhide();

        std::stringstream unhidePhrase(const std::string &);
    protected:
    private:
        bool isPixelEmpty(const Pixel &);
        void calculateNextShift();

        unsigned int shiftIndex;
        unsigned int shiftAmount;
        std::stringstream commentReaderStream;
};

#endif // STEGANOUNHIDE_H
