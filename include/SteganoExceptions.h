#ifndef PICTUREEDITEXCEPTIONS_H
#define PICTUREEDITEXCEPTIONS_H

#include <exception>

struct SteganoException {
private:
    std::string msg;
    unsigned int code;
public:
    SteganoException(const unsigned int &code, const std::string &msg) {
        this->msg = "SteganoException: " + msg;
        this->code = code;
    }

    const std::string &what() {
        return msg;
    }

    const unsigned int &errorCode() {
        return code;
    }
};

static const SteganoException img2small  (0x00000001, "Specified image too small.");
static const SteganoException imgNotFound(0x00000002, "Specified image not found.");
static const SteganoException imgNotLoaded(0x00000003, "No container image loaded.");
static const SteganoException fileStreamClosed(0x00000004, "The specified filestream for hideFile() is not open.");
static const SteganoException hideNumber2Big(0x00000005, "The input number was not valid. Check the documentation for hideNumberInMagickColorRGB().");
static const SteganoException byteUnitsError(0x00000011, "The struct ByteUnits was created with invalid units (max. 5 units, 5 tens, 2 hundreds).");

#endif // PICTUREEDIT_H
