#include "budget_manager.h"
#include "parser.h"

#include <iostream>
#include <string_view>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
	uint8_t comand_length = line.find_first_of(' ');
	Parser parser(line.substr(comand_length + 1));
	switch (comand_length)
	{
	case 4: // Earn
		manager.Earn(parser.GetDateIn(), parser.GetDateOut(), parser.GetIncome());
		break;
	case 5: // Spend
		manager.Spend(parser.GetDateIn(), parser.GetDateOut(), parser.GetIncome());
		break;
	case 6: // PayTax
		manager.PayTax(parser.GetDateIn(), parser.GetDateOut(), parser.GetTax());
		break;
	case 13: // ComputeIncome
		manager.ComputeIncome(parser.GetDateIn(), parser.GetDateOut());
		break;
	}
}

int ReadNumberOnLine(istream& input) {
	std::string line;
	std::getline(input, line);
	return std::stoi(line);
}

int main() {
	BudgetManager manager;

	const int query_count = ReadNumberOnLine(cin);

	for (int i = 0; i < query_count; ++i) {
		std::string line;
		std::getline(cin, line);
		ParseAndProcessQuery(manager, line);
	}
}
