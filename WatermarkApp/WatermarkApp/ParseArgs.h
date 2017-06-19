#pragma once
#include "pch.h"
using namespace std;

class ParseArgs
{
public:
	ParseArgs(int &argc, char **argv);
	~ParseArgs();
	const std::string& getCmdOption(const std::string &option) const;
	bool cmdOptionExists(const std::string &option) const;
	bool haveTokens();
	//const wchar_t * ParseArgs::s2wchar(const string& s);
private:
	std::vector <std::string> tokens;
	const wchar_t * s;
//		const wchar_t * s2wchar(const string& s);
};