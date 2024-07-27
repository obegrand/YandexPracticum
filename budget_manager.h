#pragma once
#include "date.h"

struct DayInfo {
	double income;
	double spend;
};

class BudgetManager {
public:
	inline static const Date START_DATE{ 2000, 1, 1 };
	inline static const Date END_DATE{ 2100, 1, 1 };

	// разработайте класс BudgetManager
	BudgetManager() 
		: all_days_(START_DATE.ComputeDistance(START_DATE, END_DATE), DayInfo()) {}

	void Earn(const Date& from, const Date& to, double income);
	
	void Spend(const Date& from, const Date& to, double value);

	void PayTax(const Date& from, const Date& to, double tax = 0.13);

	void ComputeIncome(const Date& from, const Date& to);

private:
	std::vector<DayInfo> all_days_;
};
