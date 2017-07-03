#pragma once
#include <exception>

class NoEntryException: public std::exception
{
public:
	NoEntryException();
	~NoEntryException();

	virtual const char* what() const throw()
	{
		return "No entry found with given key ";
	}
};

