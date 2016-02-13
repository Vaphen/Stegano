#include "PrivateChunk.h"

PrivateChunk::PrivateChunk(const std::string &from, const std::string &to)
{
    inputStream.open(from, std::fstream::binary);
    outputStream.open(to, std::fstream::binary);
}

PrivateChunk::~PrivateChunk()
{
    inputStream.close();
    outputStream.close();
}

/** \brief Add a new chunk to the png-file. The chunk is added in front of the IEND-chunk
 *
 * \param chunk Chunk& the chunk that should be added
 * \param data char* the data the chunk should contain
 * \param dataLength uint32_t& the length of the data (and only the length of the data)
 * \throw exception, if the given chunk was invalid
 *
 */
void PrivateChunk::addChunk(Chunk &chunk, const char *data, const uint32_t &dataLength)
{
    // check for valid png-file header
    if(!hasValidFileHeader())
    {
        throw ChunkException::PNGHeaderInvalid();
    }

    Chunk endChunk("IEND");
    std::fstream::pos_type posEndChunk = getPositionOf(endChunk);


    // copy all bytes from the input-file to the output-file until we reach the IEND-chunk
    inputStream.rdbuf()->pubseekpos(0);
    for(unsigned int i = 0; i < posEndChunk; i++)
    {
        outputStream.put(inputStream.rdbuf()->sbumpc());
    }

    // The following code is needed because it writes the bytes from the uint32_t in reversed order to the file.
    // we have to change the order.
    uint32_t inversedDataLength = endianConversation(dataLength);

    // we have to calculate the crc-sum in order to have a proper new chunk
    uint32_t calculatedCrcSum = calculateCRC(chunk.toString() + std::string(data, dataLength));

    // add the custom chunk
    outputStream.write(reinterpret_cast<const char*>(&inversedDataLength), 4);
    outputStream.write(chunk.toString().c_str(), 4);
    outputStream.write(data, dataLength);
    outputStream.write(reinterpret_cast<char*>(&calculatedCrcSum), 4);
    // this simulates the crc-checksum. We may implement a crc-check...
    //outputStream << char(0x00) << char(0x00) << char(0x00) << char(0x00);

    // finally copy the end-chunk
    while(inputStream.rdbuf()->in_avail() > 0)
    {
        outputStream.put(inputStream.rdbuf()->sbumpc());
    }
}

/** \brief Calculate the crc32-sum (for png-chunk verification)
 *
 * \param nameData const std::string& a string containing the name of the cunk and its data without separation between it.
 * (First 4 bytes are the name, the others [if any] are the data)
 * \return uint32_t the crc32-hash of the given data
 *
 */
uint32_t PrivateChunk::calculateCRC(const std::string &nameData)
{
    char *wholeData = const_cast<char*>(nameData.c_str());
    uint32_t calculatedCRC = CRC().crc(reinterpret_cast<unsigned char*>(wholeData), nameData.size());
    // the bytes are in the wrong order for output to chunk; we have to inverse them
    return endianConversation(calculatedCRC);
}

/** \brief Get the position of the specified chunk in the file (in bytes from start).
 * The position is in front of the length of the chunk.
 * \param chunk Chunk& the chunk we want to know the position of
 * \return std::fstream::pos_type the position right in front of the length of the chunk if found, else -1
 * TODO: What happens if the chunk is not found?
 */
std::fstream::pos_type PrivateChunk::getPositionOf(Chunk &chunk)
{
    // the first 8 bytes are preserved for the png-format. we can simply skip them.
    inputStream.seekg(8);

    while(inputStream.good())
    {
        char lengthOfChunk[4];
        char chunkName[4];

        // read length of chunk and its name
        inputStream.read(lengthOfChunk, 4);
        inputStream.read(chunkName, 4);

        // because the length should be an int and we got a char-array, we have to convert it
        uint32_t lengthOfChunkInBytes = convertCharArrayToInt(&lengthOfChunk[0], 4);

        // check if we found what we searched
        if(std::strncmp(&chunkName[0], chunk.toString().c_str(), 4) == 0)
        {
            // decrease current position by 8 because we've already read the next 8 bytes (name and length)
            inputStream.seekg(-8, std::ios_base::cur);
            break;
        }

        // skip current chunk size plus 4 bytes crc-value
        inputStream.seekg(lengthOfChunkInBytes + 4, std::ios_base::cur);
    }
    // the chunk is at current position if we breaked, else the chunk was not found and -1 is returned
    return inputStream.tellg();
}


/** \brief Convert a given chararray from first index to the last index into a 32-bit integer value
 *
 * \param curByte char* a pointer to the array that should be converted
 * \param &sizeOfArray const unsignedint the size of the array
 * \return uint32_t the 32-bit integer that was saved in the char-array
 */
uint32_t PrivateChunk::convertCharArrayToInt(char *curByte, const unsigned int &sizeOfArray)
{
    uint32_t lengthOfChunkInBytes = 0x00;
    for(unsigned int i = 0; i < sizeOfArray; i++)
    {
        lengthOfChunkInBytes = lengthOfChunkInBytes << 8;
        lengthOfChunkInBytes |= *reinterpret_cast<unsigned char*>(curByte);
        curByte++;
    }
    return lengthOfChunkInBytes;
}

/** \brief Get the size of a given chunks data-section
 *
 * \param chunk Chunk& the chunk thats size we want to know
 * \return uint32_t the size of the chunk
 *
 */
uint32_t PrivateChunk::getChunkSize(Chunk &chunk)
{
    // save the ifstream-position to reset it at the end of this function
    std::fstream::pos_type resetPos = inputStream.tellg();

    this->inputStream.seekg(getPositionOf(chunk));
    char lengthOfChunk[4];
    inputStream.read(lengthOfChunk, 4);

    inputStream.seekg(resetPos);
    return convertCharArrayToInt(&lengthOfChunk[0], 4);
}

/** \brief Close opened streames to make the files available for other operations
 * Must be called before the files are edited by other classes.
 */
void PrivateChunk::finish()
{
    inputStream.close();
    outputStream.close();
}


/** \brief Read a given chunks data-section into a given array.
 * The size of the array should be equal to \see getChunkSize(Chunk &chunk).
 * \param chunk Chunk& the chunk thats data-section should be read
 * \param storageArray char* a pointer to an array that is at least as big as the chunks data section
 */
void PrivateChunk::readChunk(Chunk &chunk, char *storageArray)
{
    uint32_t lengthOfChunkInBytes = getChunkSize(chunk);

    // we want to read the data so we can skip the length and name of the chunk (8 bytes)
    this->inputStream.seekg(getPositionOf(chunk));
    this->inputStream.seekg(8, std::ios_base::cur);

    // read the data
    for(unsigned int i = 0; i < lengthOfChunkInBytes; i++)
    {
        *storageArray = inputStream.get();
        storageArray++;
    }
}

/** \brief Check if the given file has a valid png file header.
 *
 * \return bool true, if the header was valid, else false
 */
bool PrivateChunk::hasValidFileHeader()
{
    char readFileHeader[8];
    const unsigned char pngFileHeader[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    this->inputStream.read(readFileHeader, 8);
    return (memcmp(readFileHeader, pngFileHeader, 8) == 0);
}

/** \brief Reverse the byte order of a given integer value
 *
 * \param input uint32_t the integer thats bytes should be inversed
 * \return uint32_t the inversed integer value
 */
uint32_t PrivateChunk::endianConversation(uint32_t input)
{
    // This strange constructs works: 01 02 03 04 -> 04 03 02 01
    return  ((input >> 24) & 0x000000FF) |
            ((input >> 8)  & 0x0000FF00) |
            ((input << 8)  & 0x00FF0000) |
            ((input << 24) & 0xFF000000);
}
