#include "SteganoHide.h"

SteganoHide::SteganoHide() : fileSize(0), doneBytes(0), outputFilePath("") { }

SteganoHide::~SteganoHide() { }

/** \brief Save the created image to disk.
 */
void SteganoHide::saveChangesToDisk() {
    if(outputFilePath.empty()) {
        throw outputFileNotSpecified;
    }
    unsigned int randNo = getRandomNumber(0, 99999999);
    this->steganoImage.write("/tmp/" + std::to_string(randNo));
    Chunk hideChunk("vaPh");
    PrivateChunk privChunk("/tmp/" + std::to_string(randNo), outputFilePath);
    privChunk.addChunk(hideChunk, this->usedPixels.c_str(), this->usedPixels.size());
    privChunk.finish();
}

/** \brief Hide a phrase (string) in the loaded picture
 *
 * \param &phraseToHide const std::string the information that should be hidden in the picture
 * \param &password const std::string a password that is not used right now
 * \throw SteganoException imgNotLoaded if no container file is loaded.
 * \throw SteganoException img2small if the loaded image is too small to hide the given phrase
 */
void SteganoHide::hidePhrase(const std::string &phraseToHide, const std::string &password) {
    if(!this->steganoImage.isValid()) {
        throw imgNotLoaded;
    }

    // we need to divide it by 2 because in worst case every byte takes 2 pixel
    if(phraseToHide.size() > (this->xResolution * this->yResolution) / 2) {
        throw img2small;
    }

    normalizeImage();

    for(unsigned int i = 0; i < phraseToHide.size(); i++) {
        Pixel hidingPixel = calculateHidingPosition(i);
     //   std::cout << hidingPixel.x << ":" << hidingPixel.y << std::endl;

        hideLetterAtPixel(phraseToHide.at(i), hidingPixel);
    }
    Pixel hidingPixel = calculateHidingPosition(phraseToHide.size());

     drawFinishPixel(hidingPixel);
}


/** \brief Hide a file (or some other ifstream) in the loaded picture
 * It loads the streamsize to this->fileSize and the amount of finished bytes to this->doneBytes
 * \param &toHideFileStream std::ifstream the filestream whose contents should be hidden in the picture (should be opened binary)
 * \param &password const std::string the password (not used yet)
 * \throw SteganoException imgNotLoaded if no container file is loaded.
 * \throw SteganoException img2small if the loaded image is too small to hide the given phrase
 * \throw SteganoException fileStreamClosed if the specified fileStream is not opened
 */
void SteganoHide::hideFile(std::ifstream &toHideFileStream, const std::string &password) {
    this->fileSize = getFileStreamSizeInBytes(toHideFileStream);

    if(!this->steganoImage.isValid()) {
        throw imgNotLoaded;
    }

    // we need to divide it by 2 because in worst case every byte takes 2 pixel
    if(this->fileSize > (this->xResolution * this->yResolution) / 2) {
        throw img2small;
    }

    if(!toHideFileStream.is_open()) {
        throw fileStreamClosed;
    }

    normalizeImage();
    unsigned int loopCount = 0;
    while(toHideFileStream.good()) {
        Pixel hidingPixel = calculateHidingPosition(loopCount);

        hideLetterAtPixel(toHideFileStream.get(), hidingPixel);

        loopCount++;
        this->doneBytes++;
   //    std::cout << "byte" << std::endl;
    }

    drawFinishPixel(calculateHidingPosition(loopCount));
}

/** \brief Calculate a free position where the nth byte should be hidden.
 *
 * \param &pixelNum const unsignedint the number of the pixel that should be hidden
 * \return Pixel a free pixel where a new byte can be stored
 */
Pixel SteganoHide::calculateHidingPosition(const unsigned int &pixelNum) {
    Pixel returnPixel(0, 0);
    int steps = quadraticSondation(pixelNum, this->xResolution * this->yResolution);
    //std::cout << "sondation faktor: " << pixelNum << " steps: " << steps << std::endl;

    pushPixelBy(returnPixel, steps);
    //std::cout << returnPixel.x << ":" << returnPixel.y << std::endl;
    Pixel nextPixel(returnPixel);
    incrementPixel(nextPixel);

    int shiftings = 0;
    while(!isPixelEmpty(returnPixel) || !isPixelEmpty(nextPixel)) {
        incrementPixel(returnPixel);
        incrementPixel(nextPixel);
        shiftings++;
    }
    // save shiftings in comments that we can restore them on exposing hidden information
    if(shiftings != 0) {
        this->usedPixels += std::to_string(pixelNum) + ":" + std::to_string(shiftings) + "\n";
        //this->steganoImage.comment(this->steganoImage.comment() + std::to_string(pixelNum) + ":" + std::to_string(shiftings) + "\n");
    }
    return returnPixel;
}


/** \brief Checks if the given Pixel is empty (not used). It is empty, if least significant bits are all 0.
 * \param pixel const Pixel& the pixel that should be checked
 * \return bool true if pixel is not used until now, else false
 */
bool SteganoHide::isPixelEmpty(const Pixel &pixel) {
    Magick::ColorRGB pixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);

    const unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.redQuantum()) % 10;
    const unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.greenQuantum()) % 10;
    const unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.blueQuantum()) % 10;
  //  std::cout << (int)redLeastSignificantBit << ":" << (int)greenLeastSignificantBit << ":" << (int)blueLeastSignificantBit << std::endl;

    return (redLeastSignificantBit == 5 && greenLeastSignificantBit == 5 && blueLeastSignificantBit == 5);
}

/** \brief Hide a letter at the specified pixelposition.
 * The letter is hidden as ASCII-value in the least-significant bits of the RGB-Value of the specified pixel.
 *
 * \param &letterToHide const unsignedchar the letter that should be hidden
 * \param &pixel Pixel a pixel-struct containing x- and y-position of the pixel where the letter should be hidden.
 * The coordinates of the pixel may change in some special cases. The pixel must be empty or it is overwritten.
 */
void SteganoHide::hideLetterAtPixel(const unsigned char &letterToHide, Pixel &pixel) {

    Magick::ColorRGB newPixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);
    if(hideNumberInMagickColorRGB(letterToHide, newPixelColor)) {
            // in this case we need just one pixel to save one byte
         this->steganoImage.pixelColor(pixel.x, pixel.y, newPixelColor);
    } else {
        // in this case we need two pixels to save one byte
        hideNumberInMagickColorRGB(calculateOverflowNumber(letterToHide), newPixelColor);
        this->steganoImage.pixelColor(pixel.x, pixel.y, newPixelColor);

        incrementPixel(pixel);

        Magick::ColorRGB overflowFirstPixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);

        hideNumberInMagickColorRGB(calculateNumberAfterOverflow(letterToHide), overflowFirstPixelColor);
        this->steganoImage.pixelColor(pixel.x, pixel.y, overflowFirstPixelColor);
    }
}

/** \brief This method calculates the overflow-number for a given letter. (First part of two pixels. The second pixel is calculated by calculateNumberAfterOverflow())
 * The overflow number always looks like: 4XY: X equals the tens-bit in byteToHide if < 5, else 5, Y same with units bit.
 * \param &byteToHide const unsignedchar the letter to hide
 * \return unsigned short a number formatted like 4XY, X and Y <= 5.
 */
unsigned short SteganoHide::calculateOverflowNumber(const unsigned char &byteToHide) {
     // overflow is always 4XY with X old green and Y old blue value.
        unsigned short overflowNumber = 400;

         if((byteToHide % 100) / 10 > 5) {
            overflowNumber += 50;
        } else {
            overflowNumber += std::floor((byteToHide % 100) / 10) * 10;
        }

        if(byteToHide % 10 > 5) {
            overflowNumber += 5;
        } else {
            overflowNumber += byteToHide % 10;
        }
        return overflowNumber;
}


/** \brief Calculate the number the pixel after the overflow-pixel has to hide (if 2 byte hiding scenario is needed).
 * The first pixel number should be calculated with calculateOverflowNumber().
 * The number after overflow always looks like: ABC:: A is the most significant bit of the byte to hide, B the middle significant bit - 5 (if less than 0, it's 0)
 * and C is the least significant bit - 5 (if less than 0, it's 0).
 * \param &byteToHide const unsignedchar the byte to hide
 * \return unsigned short
 *
 */
unsigned short SteganoHide::calculateNumberAfterOverflow(const unsigned char &byteToHide) {
    unsigned short numberAfterOverflow = std::floor(byteToHide / 100) * 100;

    if((byteToHide % 100) / 10 > 5) {
        numberAfterOverflow += (std::floor(byteToHide % 100 / 10) - 5) * 10;
    }

    if(byteToHide % 10 > 5) {
        numberAfterOverflow += (byteToHide % 10) - 5;
    }
    return numberAfterOverflow;
}


/** \brief Hide a number between with 1, 2 or 3 digits and maximum digit-number 5 in an RGB-Color.
 * Valid: 2, 34, 55, 123, 542 324, 555, ... Invalid: 6, 8, 46, 83, 464, 385, 556, 4444, ...
 * \param &smallNumber const unsignedshort a number between 0 and 555 with every digit <= 5
 * \param &color Magick::ColorRGB the color the number should be hidden in
 * \return bool true if successful, else false
 */
 // TODO: handle wrong input
bool SteganoHide::hideNumberInMagickColorRGB(const unsigned short &smallNumber, Magick::ColorRGB &color) {
    RGB pixelRGB(convert16BitTo8BitRGB(color.redQuantum()),
                 convert16BitTo8BitRGB(color.greenQuantum()),
                 convert16BitTo8BitRGB(color.blueQuantum()));

    RGB roundedRGB;
    roundedRGB.red = pixelRGB.red - (pixelRGB.red % 10);
    roundedRGB.green = pixelRGB.green - (pixelRGB.green % 10);
    roundedRGB.blue = pixelRGB.blue - (pixelRGB.blue % 10);

    unsigned char mostSignificantBit = std::floor(smallNumber / 100);
    unsigned char middleSigificantBit = std::floor(smallNumber % 100 / 10);
    unsigned char leastSignificantBit = smallNumber % 10;

   /* if(mostSignificantBit > 5 || middleSigificantBit > 5 || leastSignificantBit > 5) {
        throw hideNumber2Big;
    }*/

    // there was an overflow: eg. roundedRGB.blue = 250 and leastSignificantLetterBit = 8 => 258 => 2
    // This way to check overflow is possible, because *SignificantLetterBit >= 0
    if(roundedRGB.green + middleSigificantBit > 255) {
        return false;
    }
    if(roundedRGB.blue + leastSignificantBit > 255) {
        return false;
    }



   // std::cout << (int)roundedRGB.blue << "+" << (int)leastSignificantBit << "=";
    roundedRGB.red += mostSignificantBit;
    roundedRGB.green += middleSigificantBit;
    roundedRGB.blue += leastSignificantBit;
    //std::cout << (int)roundedRGB.blue << std::endl;

    color = Magick::ColorRGB(getRGBString(roundedRGB));
    return true;
}


/** \brief Draw a finish pixel at the specified position. The finishpixel has at the least-significant bit of their rgb-value r: 3, g: 0, b: 0
 *
 * \param &pixel const Pixel a pixel-struct describing the pixel-position where the finish-pixel should be set.
 */
void SteganoHide::drawFinishPixel(const Pixel &pixel) {
    Magick::ColorRGB pixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);

    RGB pixelRGB(convert16BitTo8BitRGB(pixelColor.redQuantum()),
                 convert16BitTo8BitRGB(pixelColor.greenQuantum()),
                 convert16BitTo8BitRGB(pixelColor.blueQuantum()));

    pixelRGB.red = (pixelRGB.red - (pixelRGB.red % 10)) + 3;
    pixelRGB.green = pixelRGB.green - (pixelRGB.green % 10);
    pixelRGB.blue = pixelRGB.blue - (pixelRGB.blue % 10);

    this->steganoImage.pixelColor(pixel.x, pixel.y, Magick::ColorRGB(getRGBString(pixelRGB)));
}


/** \brief returns the process-state in percent (value 0 to 100)
 *
 * \return unsigned char the current process-state (between 0 and 100)
 */
unsigned char SteganoHide::getDoneStateInPercent() {
    //std::cout << this->fileSize << std::endl;
    if(fileSize == 0) return 0;
    unsigned int onePercent = this->fileSize / 100;

    return this->doneBytes / onePercent;
}


/** \brief Normalize the loaded image.(set all least significant bits of all RGB-Values to 0).
 * This change takes effect only while editing the picture. In the exported image, all pixel have their old value (except the edited ones).
 * This is needed to differentiate between edited and not edited pixel.
 */
void SteganoHide::normalizeImage() {
    for(unsigned int xValue = 0; xValue < this->xResolution; xValue++) {
        for(unsigned int yValue = 0; yValue < this->yResolution; yValue++) {
            Magick::ColorRGB curPixelColor = this->steganoImage.pixelColor(xValue, yValue);


            RGB curPixelRGB(convert16BitTo8BitRGB(curPixelColor.redQuantum()),
                            convert16BitTo8BitRGB(curPixelColor.greenQuantum()),
                            convert16BitTo8BitRGB(curPixelColor.blueQuantum()));
            RGB curPixelRoundedRGB(curPixelRGB.red - (curPixelRGB.red % 10) + 5,
                                   curPixelRGB.green - (curPixelRGB.green % 10) + 5,
                                   curPixelRGB.blue - (curPixelRGB.blue % 10) + 5);
            this->steganoImage.pixelColor(xValue, yValue, Magick::ColorRGB(getRGBString(curPixelRoundedRGB)));
        }
    }
}


/** \brief Set the path to the file that should contain the steganography-picture.
 * \param outputFilePath const std::string& a path to a file; the file must not exist
 */
void SteganoHide::setOutputFilePath(const std::string &outputFilePath) {
    this->outputFilePath = outputFilePath;
}
