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


/** \brief SteganoHide can be used to hide data in images. It's designed using the singleton-patter.
 * (Get instance via SteganoHide::getInstance())
 */
class SteganoHide : public Stegano
{
public:
    virtual ~SteganoHide();
    SteganoHide(SteganoHide const&) = delete;
    void operator=(SteganoHide const&) = delete;

    void setOutputFilePath(const std::string &);
    void hidePhrase(const std::string &, const std::string &);
    void hideFile(std::ifstream &, const std::string &);
    void saveChangesToDisk();
    unsigned char getDoneStateInPercent();
    static SteganoHide &getInstance();
protected:
    SteganoHide();
private:
    void hideByteAtPixel(const unsigned char &, Pixel &);
    bool hideNumberInMagickColorRGB(const unsigned short &, Magick::ColorRGB &);
    void drawFinishPixel(const Pixel &);
    Pixel calculateHidingPosition(const unsigned int &);
    unsigned short calculateOverflowNumber(const unsigned char &);
    unsigned short calculateNumberAfterOverflow(const unsigned char &);
    bool isPixelEmpty(const Pixel &);
    void normalizeImage();
    void resetNormalizedImage();

    uint64_t hundredPercentValue;
    uint64_t doneBytes;
    std::string outputFilePath;

    Magick::Image origImageBackup;
};

#endif // STEGANOHIDE_H
