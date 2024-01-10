//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

string GetWord() {
	string str;
	string result_word;
	getline(cin, str);
	if (str.size() == 0)
	{
		return "";
	}
	else {
		result_word += str[0];
		for (size_t i = 1; i < str.size(); i++)
		{
			if (str[i]!=' ' && str[i-1]==' ')
			{
				result_word += str[i];
			}
		}
		return result_word;
	}
}

int GetIDONTKNOW() {
	int x, y;
	cin >> x >> y;
	int sum = x + y;
	int sum_size = (to_string(sum)).size();
	return sum * sum_size;
}

int main() {
	srand(time(NULL));
	int a, b;
	cin >> a >> b;
	if (a == b) { cout << "equal"s << endl; }
	else { cout << "not equal"s << endl; }
	system("pause");
}