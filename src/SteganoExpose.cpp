#include "SteganoExpose.h"

SteganoExpose::SteganoExpose() { }

SteganoExpose::~SteganoExpose() { }

// TODO: shorten this method and outsource some inner methods and add header comment
std::stringstream SteganoExpose::unhidePhrase(const std::string &password)
{
    if(!this->steganoImage.isValid())
    {
        throw SteganoException::UnhideFileNotSpecified();
    }

    this->exposeFinished = false;
    Chunk hiddenChunk("vaPh");
    std::cout << this->steganoImage.baseFilename() << std::endl;
    PrivateChunk privChunk(this->steganoImage.baseFilename(), "./omg.png");
    char *chunkData = new char[privChunk.getChunkSize(hiddenChunk)];
    privChunk.readChunk(hiddenChunk, chunkData);
    commentReaderStream.str(chunkData);
    calculateNextShift();

    Pixel curPixel(0, 0);
    std::stringstream returnPhrase;
    for(unsigned int runCounter = 0; ; runCounter++)
    {
        curPixel.x = 0;
        curPixel.y = 0;
        int_least64_t steps = quadraticSondation(runCounter, this->xResolution * this->yResolution);
        //   std::cout << "Push by" << steps << std::endl;
        pushPixelBy(curPixel, steps);
        //  std::cout << curPixel.x << ":" << curPixel.y << std::endl;

        if(runCounter == shiftIndex)
        {
            pushPixelBy(curPixel, shiftAmount);
            calculateNextShift();
        }
        // std::cout << curPixel.x << "::" << curPixel.y << std::endl;
        Magick::ColorRGB curPixelColor = this->steganoImage.pixelColor(curPixel.x, curPixel.y);

        unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.redQuantum()) % 10;
        unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.greenQuantum()) % 10;
        unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.blueQuantum()) % 10;

        // there was an overflow at the pixel color. we need to handle it special.
        unsigned char decryptedChar = 0;
        if(redLeastSignificantBit == 4)
        {
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
            incrementPixel(curPixel);

            curPixelColor = this->steganoImage.pixelColor(curPixel.x, curPixel.y);
            redLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.redQuantum()) % 10;
            greenLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.greenQuantum()) % 10;
            blueLeastSignificantBit = convert16BitTo8BitRGB(curPixelColor.blueQuantum()) % 10;
            decryptedChar += redLeastSignificantBit * 100;
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
        }
        else
        {
            decryptedChar = redLeastSignificantBit * 100;
            decryptedChar += greenLeastSignificantBit * 10;
            decryptedChar += blueLeastSignificantBit;
        }
        if(isFinishPixel(curPixel))
        {
            break;
        }
        returnPhrase << decryptedChar;
    }
    this->exposeFinished = true;
    return returnPhrase;
}

/** \brief Checks if the given Pixel is empty (not used). It is empty, if least significant bits are all 0.
 * \param pixel const Pixel& the pixel that should be checked
 * \return bool true if pixel is not used until now, else false
 */
bool SteganoExpose::isPixelEmpty(const Pixel &pixel)
{
    // not used until now
    return true;
}


/** \brief calculate the next shift index and amount out of the commentReaderStream
 * The variables shiftIndex and shiftAmount are set by a line of commentReaderStream.
 * The input should be formatted as loopIteration:skipAmount\nloopIteration:skipAmount\n....
 */
void SteganoExpose::calculateNextShift()
{
    std::string curEntry;
    std::getline(commentReaderStream, curEntry, '\n');
    size_t posOfColon = curEntry.find(':');

    if(posOfColon != std::string::npos)
    {
        shiftIndex = std::stoi(curEntry.substr(0, posOfColon));
        shiftAmount = std::stoi(curEntry.substr(posOfColon + 1, curEntry.size()));
    }
    // std::cout << shiftIndex << ":" << shiftAmount << std::endl;
}

unsigned char SteganoExpose::getDoneStateInPercent()
{
    return (exposeFinished ? 100 : 0);
}

SteganoExpose &SteganoExpose::getInstance()
{
    static SteganoExpose instance;
    return instance;
}
