#include "Stegano.h"

Stegano::Stegano() : xResolution(0), yResolution(0), pixelAmount(0) { }

Stegano::~Stegano() { }

/** \brief Load an image by its path that should be used as Stegano-Image.
 *
 * \param &imagePath const std::string a valid path to an image.
 * \throw SteganoException imgNotFound if the path was not valid or the image could not be loaded.
 */
void Stegano::loadPicture(const std::string &imagePath)
{
    try
    {
        this->steganoImage.read(imagePath);
        // We need to convert the image to gif
        // Needed because jpg compression kills accurate rgb-values
        this->steganoImage.magick("png");
        this->xResolution = this->steganoImage.size().width();
        this->yResolution = this->steganoImage.size().height();
        this->pixelAmount = this->xResolution * this->yResolution;
        //   std::cout << this->steganoImage.fileSize() << std::endl;
        //  std::cout << calculateOutputFileSize() << std::endl;
    }
    catch (Magick::ErrorBlob &loadError)
    {
        throw SteganoException::ImgNotFound();
    }
}

/** \brief Increment Pixel; if it's max x-value, x is set to 0 and y is incremented. if it's max y and max x value, both are set to 0.
 * To increment a pixel we need to know the x- and yResolution. That's why this method isn't outsourced into pixel-class.
 * \param &pixel Pixel pixel-struct containing x- and y-coordinates that should be incremented
 */
void Stegano::incrementPixel(Pixel &pixel)
{
    if(++pixel.x >= this->xResolution)
    {
        pixel.x = 0;
        if(++pixel.y >= this->yResolution)
        {
            pixel.y = 0;
        }
    }
}


/** \brief Calculate a random number between two borders (including them) in a c++11 way
 *
 * \param &from const unsignedint lower border (inclusive)
 * \param &to const unsignedint upper border (inclusive)
 * \return a random number in the given interval (including both borders)
 * \throw std::range_error if lower border is less or equal than the upper border
 */
unsigned int Stegano::getRandomNumber(const unsigned int &from, const unsigned int &to)
{
    if(from >= to)
    {
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
void Stegano::pushPixelBy(Pixel &pixel, const int &amount)
{
    // in case amount is bigger than the image Size, we shorten it
    const int shortenedAmount = mod(amount, this->pixelAmount);

    unsigned int startX = mod(pixel.x, (unsigned int)this->xResolution);
    // ySteps are the steps we need to go from current y position to reach the new one; can be positive or negative
    int ySteps = std::floor((startX + shortenedAmount) / this->xResolution);

    if(pixel.y + ySteps < 0)
    {
        // we went under 0; we have to do yResolution - |ySteps|
        pixel.y = yResolution + ySteps;
    }
    else
    {
        // we are over 0; we can add the ySteps to our current y value
        pixel.y = mod((pixel.y + ySteps), this->yResolution);
    }
    pixel.x = mod((pixel.x + shortenedAmount), (unsigned int)this->xResolution);
}

/** \brief checks if the given pixel is a finish-pixel (stegano end)
 *
 * \param &pixel const Pixel is a Pixel that should be checked for finish-attributes
 * \return bool true, if the given pixel is a finish-pixel, else false
 *
 */
bool Stegano::isFinishPixel(const Pixel &pixel)
{
    Magick::ColorRGB pixelColor = this->steganoImage.pixelColor(pixel.x, pixel.y);
    unsigned char redLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.redQuantum()) % 10;
    unsigned char greenLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.greenQuantum()) % 10;
    unsigned char blueLeastSignificantBit = convert16BitTo8BitRGB(pixelColor.blueQuantum()) % 10;
    // it's the last pixel if it has the least significant bit code r: 3, g: 0, b: 0
    return (redLeastSignificantBit == 3 && greenLeastSignificantBit == 0 && blueLeastSignificantBit == 0);
}

/** \brief Returns the size of a ifstream (the size of its file) in bytes
 * \param fileStream std::ifstream& the filestreams size (= the size of the belonging file)
 * \return unsigned int the size of the filestream in bytes
 */
unsigned int Stegano::getFileStreamSizeInBytes(std::ifstream &fileStream)
{
    auto curPos = fileStream.tellg();
    fileStream.seekg(std::ios::beg);
    auto begin = fileStream.tellg();
    fileStream.seekg (0, std::ios::end);
    auto end = fileStream.tellg();
    fileStream.seekg(curPos);
    return end - begin;
}



