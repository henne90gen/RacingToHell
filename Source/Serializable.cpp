#include "stdafx.h"
#include "Serializable.h"


#ifdef SFML_SYSTEM_WINDOWS

/**
 * Writes std::string to a stream
 * @param stream Output stream that is going to be written to
 * @param input String that will be written to the stream
 */
void Serializable::write(std::ostream &stream, std::string input) {
    unsigned long length = (unsigned long) input.size();
    stream.write((char *) &length, sizeof(length));
    stream << input;
}

/**
 * Reads std::string from a stream
 * @param stream Input stream that is going to be read from
 * @param result String that was read will be saved here
 */
void Serializable::read(std::istream &stream, std::string &result) {
    unsigned long length = 0;
    stream.read((char *) &length, sizeof(length));
    char *buffer = new char[length];
    stream.read(buffer, length);
    for (unsigned int i = 0; i < length; i++) {
        result += buffer[i];
    }
    delete[] buffer;
}

#endif

#ifdef SFML_SYSTEM_LINUX

/**
 * Writes std::string to a stream
 * @param stream Output stream that is going to be written to
 * @param input String that will be written to the stream
 */
void Serializable::write(std::ostream &stream, std::string input) {
    unsigned long length = (unsigned long) input.size();
    stream.write((char *) &length, sizeof(length));
    stream << input;
}

/**
 * Reads std::string from a stream. This only works if the length of the string is saved as unsigned long in front of
 * the string
 * @param stream Input stream that is going to be read from
 * @param result String that was read will be saved here
 */
void Serializable::read(std::istream &stream, std::string &result) {
    unsigned long length = 0;
    stream.read((char *) &length, sizeof(length));
    char *buffer = new char[length];
    stream.read(buffer, length);
    for (unsigned int i = 0; i < length; i++) {
        result += buffer[i];
    }
    delete[] buffer;
}

#endif
