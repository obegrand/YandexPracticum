//for YandexPracticum from obegrand - Andrey Melnikov
#include <iostream>
#include <ctime>

using namespace std;

void What_is_your_name();
void Calculator();

int main() {
	srand(time(NULL));
	
	Calculator();
	system("pause");
}

void What_is_your_name() {
	cout << "What is your name ?" << endl;
	string name;
	cin >> name;
	cout << "Hello, " << name << "!" << endl;
}

void Calculator()
{
	cout << "Enter x and y:" << endl;
	double x = 0, y = 0;
	cin >> x, cin >> y;
	cout << "x + y = " << x + y << endl;
	cout << "x - y = " << x - y << endl;
	cout << "x * y = " << x * y << endl;
	if (y != 0) { cout << "x / y = " << x / y << endl; }
	else { cout << "you cannot divide by 0" << endl; }
}
