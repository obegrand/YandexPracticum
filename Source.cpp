//for YandexPracticum from obegrand - Andrey Melnikov
#include <iostream>
#include <ctime>

using namespace std;

int main() {
	srand(time(NULL));
	/*for (size_t i = 0; i < 5; i++)
	{
		int x = rand() % 99 + 1;
		std::cout << "Number " << x << " is between " << x - 1 << " and " << x + 1 << ". Doubled " << x << " is " << x + x << "." << std::endl;
	}*/
	double a = 3;
	double b = 4;
	double c = 5;
	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	std::cout << "s = " << s << endl;
	system("pause");
}