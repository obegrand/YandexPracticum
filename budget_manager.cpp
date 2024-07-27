#include "budget_manager.h"
#include <iostream>
// разработайте менеджер личного бюджета самостоятельно

void BudgetManager::Earn(const Date& from, const Date& to, double income){
    const uint16_t start = from.ComputeDistance(START_DATE, from);
    const uint16_t end = to.ComputeDistance(START_DATE, to);
    const double income_per_day = income / (end - start + 1);
    for (uint16_t i = start; i <= end; ++i) {
        all_days_[i].income += income_per_day;
    }
}

void BudgetManager::Spend(const Date& from, const Date& to, double spend) {
    const uint16_t start = from.ComputeDistance(START_DATE, from);
    const uint16_t end = to.ComputeDistance(START_DATE, to);
    const double spend_per_day = spend / (end - start + 1);
    for (uint16_t i = start; i <= end; ++i) {
        all_days_[i].spend += spend_per_day;
    }
}

void BudgetManager::PayTax(const Date& from, const Date& to, double tax) {
    const uint16_t start = from.ComputeDistance(START_DATE, from);
    const uint16_t end = to.ComputeDistance(START_DATE, to);
    for (uint16_t i = start; i <= end; ++i) {
        all_days_[i].income *= 1. - (tax / 100.);
    }
}

void BudgetManager::ComputeIncome(const Date& from, const Date& to) {
    const uint16_t start = from.ComputeDistance(START_DATE, from);
    const uint16_t end = to.ComputeDistance(START_DATE, to);
    double result = 0.0;
    for (uint16_t i = start; i <= end; ++i) {
        result += all_days_[i].income - all_days_[i].spend;
    }
    std::cout << result << std::endl;
}
