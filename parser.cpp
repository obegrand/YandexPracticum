#include "parser.h"
// разработайте менеджер личного бюджета самостоятельно

const Date Parser::GetDateIn() {
	std::string_view date(line_.begin(), line_.begin() + 10);
	return Date().FromString(date);
}

const Date Parser::GetDateOut() {
	std::string_view date(line_.begin()+11, line_.begin() + 21);
	return Date().FromString(date);
}

double Parser::GetIncome() {
	return std::stoi(std::string(line_.substr(22)));
}

uint8_t Parser::GetTax() {
	return std::stoi(std::string(line_.substr(22)));
}