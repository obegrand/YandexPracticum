//for YandexPracticum from obegrand - Andrey Melnikov
#include <iostream>
#include <ctime>

using namespace std;

void What_is_your_name();
void Calculator();
void HowOldAreYou();
void SolvingComplete();
void SmartCalculator();

int main() {
	srand(time(NULL));
	/*int* arr = new int[10000000];
	system("pause");
	for (int i = 0; i < 10000000; i++)
	{if (i == 5000000)
		{
			system("pause");
		}
		arr[i] = rand() + 1000000 + 1;
	}
	delete[] arr;*/
	SmartCalculator();
	system("pause");
}

void What_is_your_name() {
	cout << "What is your name ?"s << endl;
	string name;
	cin >> name;
	cout << "Hello, "s << name << "!"s << endl;
}

void Calculator(){
	cout << "Enter x and y:"s << endl;
	double x = 0, y = 0;
	cin >> x, cin >> y;
	cout << "x + y = "s << x + y << endl;
	cout << "x - y = "s << x - y << endl;
	cout << "x * y = "s << x * y << endl;
	if (y != 0) { cout << "x / y = "s << x / y << endl; }
	else { cout << "you cannot divide by 0"s << endl; }
}

void HowOldAreYou() {
	cout << "How old are you?"s << endl;
	int age;
	cin >> age;
	if (age >= 18) {
		cout << "Welcome!"s << endl;
	}
}

void SolvingComplete() {
	double x, y, a, b;
	cin >> x, cin >> y;
	double d = x * x - 4 * y;
	if (d >= 0) {
		a = (x - sqrt(d)) / 2;
		b = (x + sqrt(d)) / 2;
		cout << a << " + "s << b << " = "s << x << endl;
		cout << a << " * "s << b << " = "s << y << endl;
	}
	else {
		cout << "No numbers with sum "s << x << " and product "s << y << endl; 
	}
	cout << "Solving complete"s << endl;
}

void SmartCalculator() {
	double x, y;
	string oper;
	cin >> x, cin >> oper, cin >> y;
	if (oper == "/" and y == 0){
		cout << "Division by zero"s << endl;
	} else if (oper != "+" and oper != "-" and oper != "*" and oper != "/") {

	}
}