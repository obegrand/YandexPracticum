#pragma once
#include "date.h"

#include <string_view>

class Parser
{
public:
	Parser(std::string_view line) : line_(line) { }

	const Date GetDateIn();
	const Date GetDateOut();
	double GetIncome();
	uint8_t GetTax();

private:
	std::string_view line_;
};