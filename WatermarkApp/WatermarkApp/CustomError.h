#pragma once

#include <iostream>
#include <exception>
using namespace std;

class CustomError : public exception
{
	mutable const char * msg;
	CustomError() {}
public:
	CustomError(const char * s) throw() : msg(s) {};
	 const char * what() const throw() { return msg; }
};

