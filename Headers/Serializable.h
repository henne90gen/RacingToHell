#pragma once
class Serializable
{
public:
	Serializable();
	~Serializable();

	virtual void serialize(std::ofstream& stream) = 0;
	virtual void deserialize(std::ifstream& stream) = 0;
};

