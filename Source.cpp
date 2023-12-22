//for YandexPracticum from obegrand - Andrey Melnikov
#include <iostream>
#include <ctime>


int main() {
	srand(time(NULL));
	for (size_t i = 0; i < 5; i++)
	{
		int x = rand() % 99 + 1;
		std::cout << "Number " << x << " is between " << x - 1 << " and " << x + 1 << ". Doubled " << x << " is " << x + x << "." << std::endl;
	}
	system("pause");
}