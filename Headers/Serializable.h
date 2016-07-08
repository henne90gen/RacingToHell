#pragma once
#include "stdafx.h"

class Serializable
{
public:
	/*
		Enables the serialization of any object. 
	*/
	Serializable() {}
	~Serializable() {}

	/*
		Writes the necessary data for an object to a stream
	*/
	virtual void operator>>(std::ostream& stream) = 0;

	/*
		Reads the necessary data for an object from a stream
	*/
	virtual void operator<<(std::istream& stream) = 0;

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
	template <typename T> void read(std::istream& stream, T& result)
	{
		stream.read((char*)&result, sizeof(result));
	}
};



