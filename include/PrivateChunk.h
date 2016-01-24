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

        std::ifstream inputStream;
        std::ofstream outputStream;
};

#endif // PRIVATECHUNK_H
