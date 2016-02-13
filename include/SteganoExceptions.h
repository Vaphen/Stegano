#ifndef PICTUREEDITEXCEPTIONS_H
#define PICTUREEDITEXCEPTIONS_H

#include <exception>


namespace SteganoException
{
class Img2Small : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The container file was too small.";
    }
};

class ImgNotFound : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The specified image could not be found.";
    }
};

class ImgNotLoaded : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: No container file loaded.";
    }
};

class FileStreamClosed : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The specified filestream for hideFile() is not open.";
    }
};

class HideNumber2Big : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The input number was not valid. Check the documentation for hideNumberInMagickColorRGB().";
    }
};

class ByteUnitsError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The struct ByteUnits was created with invalid units (max. 5 units, 5 tens, 2 hundreds).";
    }
};

class OutputFileNotSpecified : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: You perform some action on the outputfile without specifieng it. Call 'setOutputFilePath()' first.";
    }
};

class UnhideFileNotSpecified : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: You tried to perform the unhide action with unset container-/outputfilepath.";
    }
};

class HideFileNotExistant : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The file that should be hidden cannot be found.";
    }
};

class OutputFileStreamNotGood : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SteganoError: The output file cannot be opened.\nHave you write-permissions?";
    }
};
}
#endif // PICTUREEDIT_H
