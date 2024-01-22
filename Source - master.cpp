//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <map>
#include <string>

using namespace std;

void PrintMap(const map<char, int>& map_tmp) {
    for (const auto& [key, value] : map_tmp){
        cout << key << " - " << value << endl;
    }
}

map<char, int> GetAnagram(const string& str) {
    map<char, int> tmp_map;
    for (const char& tmp_char : str) {
        ++tmp_map[tmp_char];
    }
    PrintMap(tmp_map);
    return tmp_map;
}

bool CheckAnagram(const string& s1, const string& s2) {
    if (GetAnagram(s1) == GetAnagram(s2)){
        return 1;
    }
    else {
        return 0;
    }
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

    system("pause");
    //CheckIsTrue(CheckAnagram("tea"s, "eat"s));
    //CheckIsTrue(!CheckAnagram("battle"s, "beatle"s));
    //CheckIsTrue(!CheckAnagram("lift"s, "elevator"s));
    //CheckIsTrue(CheckAnagram("ocean"s, "canoe"s));
}