#include "stdafx.h"
#include "CarFactory.h"


CarFactory::CarFactory()
{
}


CarFactory::~CarFactory()
{
}

Car CarFactory::getCar(Cars type)
{
	Car car;
	switch (type) {
	case PlayerCar:

	case Cars::AICar:
		car = Car(50, 2000, type);
	case Cars::Truck:
		car = Car(200, 500, type);
		break;
	}

	return car;
}
