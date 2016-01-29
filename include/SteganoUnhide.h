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
        virtual ~SteganoUnhide();

        std::stringstream unhidePhrase(const std::string &);
        unsigned char getDoneStateInPercent();

        SteganoUnhide(SteganoUnhide const&) = delete;
        void operator=(SteganoUnhide const&) = delete;
        static SteganoUnhide &getInstance();
    protected:
        SteganoUnhide();
    private:
        bool isPixelEmpty(const Pixel &);
        void calculateNextShift();

        unsigned int shiftIndex;
        unsigned int shiftAmount;
        std::stringstream commentReaderStream;

        bool exposeFinished;
};

#endif // STEGANOUNHIDE_H
