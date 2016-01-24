#ifndef CHUNKEXCEPTIONS_H_
#define CHUNKEXCEPTIONS_H_

#include <exception>

namespace ChunkException {
    class PNGHeaderInvalid : public std::exception {
        virtual const char* what() const throw() {
            return "ChunkError: The input file was not a valid png file.";
        }
    };

    class ChunkNameSizeInvalid : public std::exception {
        virtual const char* what() const throw() {
            return "ChunkError: The name of the chunk must be 4 characters long (Protip: read about png-chunks before using them).";
        }
    };

    class ChunkNameInvalid : public std::exception {
        virtual const char* what() const throw() {
            return "ChunkError: Just alphabetical letters are allowed as chunk name (Protip: read about png-chunks before using them).";
        }
    };
}
#endif // CHUNKEXCEPTIONS_H_
