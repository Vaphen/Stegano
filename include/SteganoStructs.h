#ifndef STEGANOSTRUCTS_H_INCLUDED
#define STEGANOSTRUCTS_H_INCLUDED

struct Pixel
{
    Pixel(const unsigned int &x, const unsigned int &y)
    {
        this->x = x;
        this->y = y;
    }
    Pixel(const Pixel &pixel)
    {
        this->x = pixel.x;
        this->y = pixel.y;
    }

    Pixel() = default;

    inline bool operator == (const Pixel &pixel) const
    {
        return (this->x == pixel.x && this->y == pixel.y);
    }

    bool operator < (const Pixel &pixel) const
    {
        if(this->y == pixel.y)
        {
            return (this->x < pixel.x);
        }
        else
        {
            return (this->y < pixel.y);
        }
    }
    unsigned int x;
    unsigned int y;
};

struct RGB
{
    RGB(const unsigned char &red, const unsigned char &green, const unsigned char &blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    RGB() = default;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    std::string toString() const
    {
        return "rgb(" + std::to_string(red) + "," + std::to_string(green) + "," + std::to_string(blue) + ")";
    }
};

struct ByteUnits
{
    ByteUnits(const unsigned char &units, const unsigned char &tens, const unsigned char &hundreds)
    {
        if(hundreds > 2 || tens > 5 || units > 5)
        {
            throw SteganoException::ByteUnitsError();
        }
        this->units = units;
        this->tens = tens;
        this->hundreds = hundreds;
    }

    ByteUnits(const unsigned char &byte)
    {
        this->units = byte % 10;
        this->tens = byte % 100 / 10;
        this->hundreds = byte / 100;
    }

    ByteUnits() = default;

    unsigned char toByte()
    {
        return this->hundreds * 100 + this->tens * 10 + this->units;
    }

    unsigned char units;
    unsigned char tens;
    unsigned char hundreds;
};

#endif // STEGANOSTRUCTS_H_INCLUDED
