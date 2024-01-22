//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    map<string, int> days_here = { {"Karl"s, 10}, {"Gustav"s, 3}, {"Richard"s, 42}, {"Wolfgang"s, 15} };
    {
        map<string, int> days_here_new;
        for (const auto& [name, days] : days_here) {
            days_here_new["Mr. "s + name] = days;
        }
        days_here = days_here_new;
    }
    cout << "Mr. Karl has been here for "s << days_here["Mr. Karl"s] << " days"s << endl;
    system("pause");
}