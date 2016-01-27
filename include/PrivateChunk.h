#ifndef PRIVATECHUNK_H
#define PRIVATECHUNK_H

#include <fstream>
#include <iostream>
#include <regex>
#include "ChunkExceptions.h"

struct Chunk {
    // The 4 bytes describing one chunk
    char ancillary_byte;
    char private_byte;
    char reserved_byte;
    char saftToCopy_byte;


    /** \brief A Chunk should be cread with a name. The name must be exactly 4 chars long.
     * If you don't know how the naming-conventions for a chunk are, google it.
     * \param chunkName const std::string& an alphabetical, 4 bytes long name of either an existing chunk or a new one
     * \throw exception if the chunk name was not valid
     */
    Chunk(const std::string &chunkName) {
        // check size of chunk
        if(chunkName.size() != 4) {
            throw ChunkException::ChunkNameSizeInvalid();
        }

        // check if chunk are just normal letters
        if(!std::regex_match(chunkName, std::regex("^[A-Za-z]+$"))) {
            throw ChunkException::ChunkNameInvalid();
        }

        this->ancillary_byte = chunkName.at(0);
        this->private_byte = chunkName.at(1);
        this->reserved_byte = chunkName.at(2);
        this->saftToCopy_byte = chunkName.at(3);
    }


    /** \brief Convert the chunk back to a 4 byte string.
     * \return std::string the chunks name (4 bytes)
     */
    std::string toString() {
        std::string returnString;
        returnString.push_back(ancillary_byte);
        returnString.push_back(private_byte);
        returnString.push_back(reserved_byte);
        returnString.push_back(saftToCopy_byte);
        return returnString;
    }
};


class PrivateChunk {
    public:
        PrivateChunk(const std::string &, const std::string &);
        virtual ~PrivateChunk();

        void addChunk(Chunk &, const char *, const uint32_t &);
        Chunk getChunk(const std::string &);
        uint32_t getChunkSize(Chunk &);
        void readChunk(Chunk &, char *);
        void finish();
    protected:
    private:
        bool hasValidFileHeader();
        std::fstream::pos_type getPositionOf(Chunk &);
        uint32_t convertCharArrayToInt(char *, const unsigned int &);
        uint32_t endianConversation(uint32_t);
        uint32_t calculateCRC(const std::string &);

        std::ifstream inputStream;
        std::ofstream outputStream;
};


/**
 * \brief This crc-calculation was found at https://www.w3.org/TR/PNG-CRCAppendix.html and modified to work
 * a bit better in a c++ way. We don't have to understand what it's doing; however it works.
 */
struct CRC {
    private:
       uint32_t crcTable[256];

       bool crcTableComputed = false;

       void makeCrcTable() {
         uint32_t curTableValue;

         for (unsigned int outerLoopCounter = 0; outerLoopCounter < 256; outerLoopCounter++) {
           curTableValue = outerLoopCounter;
           for (unsigned int innerLoopCount = 0; innerLoopCount < 8; innerLoopCount++) {
             if (curTableValue & 1) {
               curTableValue = 0xedb88320L ^ (curTableValue >> 1);
             } else {
               curTableValue = curTableValue >> 1;
             }
           }
           crcTable[outerLoopCounter] = curTableValue;
         }
         crcTableComputed = true;
       }

       uint32_t updateCrc(uint32_t crc, unsigned char *buf,
                                unsigned int len)
       {
         uint32_t currentCRC = crc;

         if (!crcTableComputed) {
           makeCrcTable();
         }

         for (unsigned int i = 0; i < len; i++) {
           currentCRC = crcTable[(currentCRC ^ buf[i]) & 0xff] ^ (currentCRC >> 8);
         }
         return currentCRC;
       }
    public:
       uint32_t crc(unsigned char *buf, int len)
       {
         return updateCrc(0xffffffffL, buf, len) ^ 0xffffffffL;
       }
};

#endif // PRIVATECHUNK_H
