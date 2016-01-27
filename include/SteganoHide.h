#ifndef STEGANOHIDE_H
#define STEGANOHIDE_H

#include <string>
#include <Magick++.h>
#include <iostream>
#include <fstream>
#include "SteganoExceptions.h"
#include "SteganoStructs.h"
#include "Stegano.h"
#include "PrivateChunk.h"

class SteganoHide : public Stegano
{
    public:
        SteganoHide();
        virtual ~SteganoHide();


        void setOutputFilePath(const std::string &);
        void hidePhrase(const std::string &, const std::string &);
        void hideFile(std::ifstream &, const std::string &);
        void saveChangesToDisk();
        unsigned char getDoneStateInPercent();
    protected:
    private:
        void hideLetterAtPixel(const unsigned char &, Pixel &);
        bool hideNumberInMagickColorRGB(const unsigned short &, Magick::ColorRGB &);
        void drawFinishPixel(const Pixel &);
        Pixel calculateHidingPosition(const unsigned int &);
        unsigned short calculateOverflowNumber(const unsigned char &);
        unsigned short calculateNumberAfterOverflow(const unsigned char &);
        bool isPixelEmpty(const Pixel &);
        void normalizeImage();


        unsigned int fileSize;
        unsigned int doneBytes;
        std::string outputFilePath;
};

#endif // STEGANOHIDE_H
