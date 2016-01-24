#include "SteganoUnhide.h"

SteganoUnhide::SteganoUnhide() { }

SteganoUnhide::~SteganoUnhide() { }

// TODO: shorten this method and outsource some inner methods
std::stringstream SteganoUnhide::unhidePhrase(const std::string &password) {
    Pixel curPixel(0, 0);
    std::stringstream returnPhrase;
    for(unsigned int runCounter = 0; ;runCounter++) {
        curPixel.x = 0;
        curPixel.y = 0;
        int steps = quadraticSondation(runCounter, this->xResolution * this->yResolution);
       // std::cout << "Push by" << steps << std::endl;
        pushPixelBy(curPixel, steps);
      //  std::cout << curPixel.x << ":" << curPixel.y << std::endl;

        Pixel nextPixel(curPixel);
        incrementPixel(nextPixel);
        while(!isPixelEmpty(curPixel) || !isPixelEmpty(nextPixel)) {
            incrementPixel(curPixel);
            incrementPixel(nextPixel);
        }
        std::cout << curPixel.x << ":" << curPixel.y << std::endl;
        Magick::ColorRGB curPixelColor = this->steganoImage.pixelColor(curPixel.x, curPixel.y);

        unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.redQuantum()) % 10;
        unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.greenQuantum()) % 10;
        unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.blueQuantum()) % 10;

        // save that we already decrypted the current Pixel
        usedPixels.insert(Pixel(curPixel));

        // there was an overflow at the pixel color. we need to handle it special.
        unsigned char decryptedChar = 0;
        if(redLeastSignificantBit == 4) {
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
            incrementPixel(curPixel);

            usedPixels.insert(Pixel(curPixel));
            curPixelColor = this->steganoImage.pixelColor(curPixel.x, curPixel.y);
            redLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.redQuantum()) % 10;
            greenLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.greenQuantum()) % 10;
            blueLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.blueQuantum()) % 10;
            decryptedChar += redLeastSignificantBit * 100;
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
        } else {
            decryptedChar = redLeastSignificantBit * 100;
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
        }
        if(isFinishPixel(curPixel)) {
            break;
        }
        returnPhrase << decryptedChar;
    }
    return returnPhrase;
}

ÿ