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

class SteganoExpose : public Stegano
{
public:
    virtual ~SteganoExpose();

    std::stringstream unhidePhrase(const std::string &);
    unsigned char getDoneStateInPercent();

    SteganoExpose(SteganoExpose const&) = delete;
    void operator=(SteganoExpose const&) = delete;
    static SteganoExpose &getInstance();
protected:
    SteganoExpose();
private:
    bool isPixelEmpty(const Pixel &);
    void calculateNextShift();

    unsigned int shiftIndex;
    unsigned int shiftAmount;
    std::stringstream commentReaderStream;

    bool exposeFinished;
};

#endif // STEGANOUNHIDE_H
