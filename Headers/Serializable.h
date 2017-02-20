
#pragma once

class Serializable {
public:

    Serializable() {}

    virtual ~Serializable() {}

    /**
     * Writes the necessary data for an object to a packet
     */
    virtual void operator>>(sf::Packet &packet) {}

    /**
     * Reads the necessary data for an object from a packet
     */
    virtual void operator<<(sf::Packet &packet) {}

protected:

//   Template methods have to be defined right away. Don't move the definition into the .cpp file.
//   Overriding template methods is done without the template<typename T> modifier.
//   Just write a normal method and everything is fine. :)

    /**
     * Writes std::string to a stream
     * @param stream Output stream that is going to be written to
     * @param input String that will be written to the stream
     */
    void write(std::ostream &stream, std::string input);

    /**
     * Reads std::string from a stream
     * @param stream Input stream that is going to be read from
     * @param result String that was read will be saved here
     */
    void read(std::istream &stream, std::string &result);

    /**
     * Writes any data (except std::string) to a stream
     * @param stream Output stream that is going to be written to
     * @param input Primitive type that will be written to a stream
     */
    template<typename T>
    void write(sf::Packet &packet, T input) {
        packet << input;
    }

    /**
     * Reads any data (except std::string) from a stream
     * @param stream Input stream that is going to be read from
     * @param result Data that was read will be saved here
     */
    template<typename T>
    void read(sf::Packet &packet, T &result) {
        packet >> result;
    }

    /**
     * Writes any data (except std::string) to a stream
     * @param stream Output stream that is going to be written to
     * @param input Primitive type that will be written to a stream
     */
    template<typename T>
    void write(std::ostream &stream, T input) {
        stream.write((char *) &input, sizeof(input));
    }

    /**
     * Reads any data (except std::string) from a stream
     * @param stream Input stream that is going to be read from
     * @param result Data that was read will be saved here
     */
    template<typename T>
    void read(std::istream &stream, T &result) {
        stream.read((char *) &result, sizeof(result));
    }
};
