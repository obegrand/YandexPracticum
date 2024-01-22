//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <map>
#include <string>

using namespace std;

bool CheckAnagram(const string& s1, const string& s2) {
    // напишите решение тут
}

void CheckIsTrue(bool value) {
    if (value) {
        cout << "Test passed :)"s << endl;
    }
    else {
        cout << "Test failed :("s << endl;
    }
}

int main() {
    CheckIsTrue(CheckAnagram("tea"s, "eat"s));
    CheckIsTrue(!CheckAnagram("battle"s, "beatle"s));
    CheckIsTrue(!CheckAnagram("lift"s, "elevator"s));
    CheckIsTrue(CheckAnagram("ocean"s, "canoe"s));
}