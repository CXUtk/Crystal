#pragma once
#include "test.h"
class Internal
{
public:
	int A;
	float B;
	constexpr static auto properties()
	{
		return std::make_tuple(
			PROPERTY(Internal, A),
			PROPERTY(Internal, B)
		);
	}
};

class TestObject
{
public:
	int			Age;
	float		Weight;
	std::string	Name;
	bool		IsMale;
	Internal	Inter;
	int* s;

	constexpr static auto properties()
	{
		return std::make_tuple(
			PROPERTY(TestObject, Age),
			PROPERTY(TestObject, Weight),
			PROPERTY(TestObject, Name),
			PROPERTY(TestObject, IsMale),
			PROPERTY(TestObject, Inter),
			PROPERTY(TestObject, s)
		);
	}
};
