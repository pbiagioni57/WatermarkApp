#include "stdafx.h"
#include "ParseArgs.h"

ParseArgs::ParseArgs(int &argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		this->tokens.push_back(std::string(argv[i]));
}

ParseArgs::~ParseArgs()
{
}

const std::string& ParseArgs::getCmdOption(const std::string &option) const {
	std::vector<std::string>::const_iterator itr;
	itr = std::find(this->tokens.begin(), this->tokens.end(), option /*tolower(option[1])*/);
	if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
		return *itr;
	}
	static const std::string empty_string("");
	return empty_string;
}

bool ParseArgs::cmdOptionExists(const std::string &option) const {
	return std::find(this->tokens.begin(), this->tokens.end(), option)
		!= this->tokens.end();
}

bool ParseArgs::haveTokens()
{
	return ((this->tokens.size() == 0) ? false : true);
}
