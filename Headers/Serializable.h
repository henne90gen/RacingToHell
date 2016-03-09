#pragma once
class Serializable
{
public:
	Serializable();
	~Serializable();

	virtual void serialize(std::ostream& stream) = 0;
	virtual void deserialize(std::istream& stream) = 0;
};

