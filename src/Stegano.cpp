#include "Stegano.h"

Stegano::Stegano() { }

Stegano::~Stegano() { }

/** \brief Load an image by its path that should be used as Stegano-Image.
 *
 * \param &imagePath const std::string a valid path to an image.
 * \throw SteganoException imgNotFound if the path was not valid or the image could not be loaded.
 */
void Stegano::loadPicture(const std::string &imagePath) {
    try {
        this->steganoImage.read(imagePath);
        // We need to convert the image to gif
        // Needed because jpg compression kills accurate rgb-values
        this->steganoImage.magick("png");
        this->xResolution = this->steganoImage.size().width();
        this->yResolution = this->steganoImage.size().height();
     //   std::cout << this->steganoImage.fileSize() << std::endl;
      //  std::cout << calculateOutputFileSize() << std::endl;
    }catch (Magick::ErrorBlob &loadError) {
        throw imgNotFound;
    }
}

/** \brief Increment Pixel; if it's max x-value, x is set to 0 and y is incremented. if it's max y and max x value, both are set to 0.
 *
 * \param &pixel Pixel pixel-struct containing x- and y-coordinates that should be incremented
 */
void Stegano::incrementPixel(Pixel &pixel) {
    if(++pixel.x >= this->xResolution) {
        pixel.x = 0;
        if(++pixel.y >= this->yResolution) {
            pixel.y = 0;
        }
    }
}

/** \brief Create a RGB-String for ImageMagick formatted like "rgb(255, 97, 6)"
 *
 * \param &rgb const RGB a rgb-struct containing the colors that should be used to create the string
 * \return std::string a rgb-string formatted like "rgb(255, 255, 255)"
 */
std::string Stegano::getRGBString(const RGB &rgb) {
    return "rgb(" + std::to_string(rgb.red) + "," + std::to_string(rgb.green) + "," + std::to_string(rgb.blue) + ")";
}


/** \brief Calculate a random number between two borders (including them) in a c++11 way
 *
 * \param &from const unsignedint lower border (inclusive)
 * \param &to const unsignedint upper border (inclusive)
 * \return a random number in the given interval (including both borders)
 * \throw std::range_error if lower border is less or equal than the upper border
 */
unsigned int Stegano::getRandomNumber(const unsigned int &from, const unsigned int &to) {
    if(from >= to) {
        throw std::range_error("The lower border must be less than the upper border when calling getRandomNumber");
    }
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(from, to);
    return dis(gen);
}


/** \brief Fill up x- and y-coords of the given image (first x then y) with a special amount.
 *         In case of overflow it continues with the first pixel.
 * \param &pixel Pixel the pixel the amount should be added
 * \param &amount const int amount of pixels that should be added to the given pixel
 */
void Stegano::pushPixelBy(Pixel &pixel, const int &amount) {
    unsigned int pixelInImageAmount = this->yResolution * this->xResolution;

    // in case amount is bigger than the image Size, we shorten it
    const int shortenedAmount = mod(amount, pixelInImageAmount);

    unsigned int xResolution = (unsigned int)this->xResolution;
    unsigned int startX = mod(pixel.x, xResolution);
    // ySteps are the steps we need to go from current y position to reach the new one; can be positive or negative
    int ySteps = std::floor((startX + shortenedAmount) / xResolution);

    if(pixel.y + ySteps < 0) {
        // we went under 0; we have to do yResolution - |ySteps|
        pixel.y = yResolution + ySteps;
    } else {
        // we are over 0; we can add the ySteps to our current y value
        pixel.y = mod((pixel.y + ySteps), this->yResolution);
    }
    pixel.x = mod((pixel.x + shortenedAmount), xResolution);
}


/** \brief This mod function is needed because c++ standard mod operator (%) doesn't work properly in a mathematical way on negative numbers.
 * The result of mod(-6, 4) would be 2, mod(9, 3) = 0, mod(8, -3) = -1, ...
 * \param &x const int the value that should be taken modulo
 * \param &m const int the modulo value
 * \return inline int result of x mod m
 */
inline int Stegano::mod(const int &x, const int &m) {
    return (x%m + m)%m;
}

/** \brief convert ImageMagick rgb values from default 16-bit (max. 65536) value to 8 bit value (max. 255)
 *
 * \param &rgb16Bit const unsignedint a rgb value in 16 bit
 * \return unsigned char a valid rgb value in range of 0 to 255 (including 0 and 255)
 */
unsigned char Stegano::convert16BitTo8BitRGB(const unsigned int &rgb16Bit) {
    return rgb16Bit / 257;
}


/** \brief checks if the given pixel is a finish-pixel (stegano end)
 *
 * \param &pixel const Pixel is a Pixel that should be checked for finish-attributes
 * \return bool true, if the given pixel is a finish-pixel, else false
 *
 */
bool Stegano::isFinishPixel(const Pixel &pixel) {
    Magick::ColorRGB pixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);
    unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.redQuantum()) % 10;
    unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.greenQuantum()) % 10;
    unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.blueQuantum()) % 10;
    // it's the last pixel if it has the least significant bit code r: 3, g: 0, b: 0
    return (redLeastSignificantBit == 3 && greenLeastSignificantBit == 0 && blueLeastSignificantBit == 0);
}


/** \brief calculate quadratic sondation of a given index; it is always lower than maxSize.
 * Collisions are possible
 * \param &index const unsignedint the index that should be sondated
 * \param &maxSize const unsignedint the maximum size of the return-value
 * \return int the result of the quadratic sondation
 */
int_least64_t Stegano::quadraticSondation(const unsigned int &index, const unsigned int &maxSize) {
  //  std::cout << "faktor :" << index << " - " << "sondation: " << (int)std::pow(-1, index) * ((int)std::pow(index, 2) % maxSize) << std::endl;
    return (int_least64_t)std::pow(-1, index) * ((int_least64_t)std::pow(index, 2) % maxSize);
}

/** \brief Checks if the given Pixel is empty (not used). It is empty, if least significant bits are all 0.
 * \param pixel const Pixel& the pixel that should be checked
 * \return bool true if pixel is not used until now, else false
 */
bool Stegano::isPixelEmpty(const Pixel &pixel) {
    Magick::ColorRGB pixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);

    const unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.redQuantum()) % 10;
    const unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.greenQuantum()) % 10;
    const unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.blueQuantum()) % 10;
  //  std::cout << (int)redLeastSignificantBit << ":" << (int)greenLeastSignificantBit << ":" << (int)blueLeastSignificantBit << std::endl;

    return (redLeastSignificantBit == 0 && greenLeastSignificantBit == 0 && blueLeastSignificantBit == 0);
}

/** \brief Returns the size of a ifstream (the size of its file) in bytes
 * \param fileStream std::ifstream& the filestreams size (= the size of the belonging file)
 * \return unsigned int the size of the filestream in bytes
 */
unsigned int Stegano::getFileStreamSizeInBytes(std::ifstream &fileStream) {
    auto curPos = fileStream.tellg();
    auto begin = fileStream.tellg();
    fileStream.seekg (0, std::ios::end);
    auto end = fileStream.tellg();
    fileStream.seekg(curPos);
    return end - begin;
}



