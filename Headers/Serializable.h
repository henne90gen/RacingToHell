#pragma once

class Serializable
{
public:
	/*
		Enables the serialization of any object. 
	*/
	Serializable() {}
	virtual ~Serializable() {}

	/*
		Writes the necessary data for an object to a stream
	*/
	virtual void operator>>(std::ostream& stream) = 0;

	/*
		Reads the necessary data for an object from a stream
	*/
	virtual void operator<<(std::istream& stream) = 0;

	/*
		Writes the necessary data for an object to a packet
	*/
	virtual void operator>>(sf::Packet& packet) = 0;

	/*
		Reads the necessary data for an object from a packet
	*/
	virtual void operator<<(sf::Packet& packet) = 0;

protected:

	/*
		Writes any data (except std::string) to a stream
		@param stream Output stream that is going to be written to
		@param input Primitive type that will be written to a stream
	*/
	template <typename T> void write(sf::Packet& packet, T input)
	{
		packet << input;
	}

	/*
		Reads any data (except std::string) from a stream
		@param stream Input stream that is going to be read from
		@param result Data that was read will be saved here
	*/
	template <typename T> void read(sf::Packet& packet, T &result)
	{
		packet >> result;
	}

	/*
		Writes any data (except std::string) to a stream
		@param stream Output stream that is going to be written to
		@param input Primitive type that will be written to a stream
	*/
	template <typename T> void write(std::ostream& stream, T input)
	{
		stream.write((char*)&input, sizeof(input));
	}

	/*
		Reads any data (except std::string) from a stream
		@param stream Input stream that is going to be read from
		@param result Data that was read will be saved here
	*/
	template <typename T> void read(std::istream& stream, T &result)
	{
		stream.read((char*)&result, sizeof(result));
	}

	#ifndef SFML_SYSTEM_WINDOWS

	/*
	Writes std::string to a stream
	@param stream Output stream that is going to be written to
	@param input String that will be written to the stream
	*/
	
	template <> void Serializable::write(std::ostream& stream, std::string input)
	{
		int length = input.size();
		stream.write((char*)&length, sizeof(length));
		stream << input;
	} 

	/*
	Reads std::string from a stream
	@param stream Input stream that is going to be read from
	@param result String that was read will be saved here
	*/
	template <> void Serializable::read(std::istream& stream, std::string& result)
	{
		int length;
		stream.read((char*)&length, sizeof(length));
		char * buffer = new char[length];
		stream.read(buffer, length);
		for (unsigned int i = 0; i < length; i++) {
			result += buffer[i];
		}
		delete[] buffer;
	}
	#endif
};

