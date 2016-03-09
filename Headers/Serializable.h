#pragma once
#include "stdafx.h"

class Serializable
{
public:
	Serializable() {}
	~Serializable() {}

	//virtual void operator<<(std::ofstream& stream) = 0;
	//virtual void operator>>(std::ifstream& stream) = 0;

	template <typename T> void write(std::ofstream& stream, T input)
	{
		stream.write((char*)&input, sizeof(input));
	}

	template <typename T> void read(std::ifstream& stream, T& result)
	{
		stream.read((char*)&result, sizeof(result));
	}

	template <> void write(std::ofstream& stream, std::string input)
	{
		int length = input.size();
		stream.write((char*)&length, sizeof(length));
		stream << input;
	}
	template <> void read(std::ifstream& stream, std::string& result)
	{
		int length;
		stream.read((char*)&length, sizeof(length));
		char* newString = new char[length];
		stream.read(newString, length);
		for (int i = 0; i < length; i++) {
			result += newString[i];
		}
	}
};

