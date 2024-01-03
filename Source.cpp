//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

void What_is_your_name();
void Calculator();
void HowOldAreYou();
void SolvingComplete();
void SmartCalculator();
void WhatIsTriangler();
void First1st();
void SwapVector5numbers();
void ReversedVector();

vector<string> some_vector = { "one", "two", "three", "four", "five" };

int main() {
	srand(time(NULL));
	for (string str : some_vector) {
		cout << str << ", ";
	}
	cout << endl;
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
	if (oper == "+") 
	{
		cout << "= " << x + y << endl;
	}
	else if (oper == "-")
	{
		cout << "= " << x - y << endl;
	}
	else if (oper == "*")
	{
		cout << "= " << x * y << endl;
	}
	else if (oper == "/")
	{
		if (y == 0)
		{
			cout << "Division by zero" << endl;
		}
		else
		{
			cout << "= " << x / y << endl;
		}
	}
	else
	{
		cout << "Unknown operation: " << oper << endl;
	}
}

void WhatIsTriangler() {
	int a, b, c = 0;
	cin >> a >> b >> c;
	if (a + b > c && a + c > b && b + c > a){
		if (a == b && b == c && a == c) {
			cout << "equilateral triangle" << endl;
		}
		else if (a == b || b == c || a == c) {
			cout << "isosceles triangle" << endl;
		}
		else { cout << "common triangle" << endl; }
	}
	else { cout << "incorrect triangle" << endl; }
}

void First1st() {
	int number, leftover, leftover10;
	string sufex;
	bool minus;
	cin >> number;
	if (number < 0) { minus = true; } else { minus = false; }
	number = abs(number);
	leftover = number % 10;
	leftover10 = number % 100;
	if (leftover10 == 11 || leftover10 == 12 || leftover10 == 13)
	{
		sufex = "th";
	}
	else if (leftover == 1)
	{
		sufex = "st";
	}
	else if (leftover == 2)
	{
		sufex = "nd";
	}
	else if (leftover == 3)
	{
		sufex = "rd";
	}
	else { sufex = "th"; }
	if (!minus) { cout << number << sufex << endl; }
	else { cout << "-" << number << sufex << endl; }
}

void SwapVector5numbers(){
	vector<int> five_nums = { 100, 28, 37, 66, 16536 };
	for (int i = 0; i < five_nums.size(); i++) { cout << five_nums[i] << " "; }cout << endl;
	int tmpSwap;
	tmpSwap = five_nums[4];
	five_nums[4] = five_nums[0];
	five_nums[0] = tmpSwap;
	tmpSwap = five_nums[3];
	five_nums[3] = five_nums[1];
	five_nums[1] = tmpSwap;
	for (int i = 0; i < five_nums.size(); i++) { cout << five_nums[i] << " "; }cout << endl;
}

void ReversedVector() {
	vector<int> some_vector = { 2, 4, 8, 16 };
	vector<int> reversed_vector;
	for (int i = 0; i < some_vector.size(); i++) { cout << some_vector[i] << " "; }cout << endl;
	for (int i = some_vector.size(); i > 0; i--) {
		reversed_vector.push_back(some_vector[i - 1]);
	}
	some_vector.clear();
	for (int i = 0; i < reversed_vector.size(); i++) { cout << reversed_vector[i] << " "; }cout << endl;
}