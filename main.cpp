#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// не мен€йте файлы xml.h и xml.cpp
#include "xml.h"
#include "json.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(spendings.begin(), spendings.end(), 0,
        [](int current, const Spending& spending) {
            return current + spending.amount;
        });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
        };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    using namespace json;
    Document json(Load(input));
    const Node& rootNode = json.GetRoot();

    vector<Node> jsonArray = rootNode.AsArray();
    vector<Spending> spendings;

    for (const Node& node : jsonArray) {
        map<string, Node> jsonObject = node.AsMap();
        string category = jsonObject.at("category").AsString();
        int amount = jsonObject.at("amount").AsInt();
        spendings.emplace_back(category, amount);
    }

    return spendings;
}

vector<Spending> LoadFromXml(istream& input) {
    using namespace xml;
    Document xml(Load(input));
    const Node& rootNode = xml.GetRoot();
    vector<Spending> spendings;

    for (const Node& child : rootNode.Children()) {
        string category = child.AttributeValue<string>("category");
        int amount = child.AttributeValue<int>("amount");
        spendings.emplace_back(category, amount);

    }

    return spendings;
}

int main() {
    const vector<Spending> spendings = LoadFromXml(cin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}