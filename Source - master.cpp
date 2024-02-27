#include <numeric>
#include <ostream>
#include <vector>
#include <iostream>

using namespace std;

// Класс Дроби в виде *numerator* / *denominator*
class Rational {
public:
    // создание дроби 0/1
    Rational() = default;

    // создание дроби *numerator*/1 
    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    // создание дроби *numerator* / *denominator*
    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    // получение *numerator*
    int Numerator() const {
        return numerator_;
    }

    // получение *denominator*
    int Denominator() const {
        return denominator_;
    }
private:
    // приведение к НОД(Наибольшему общему делителю)
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

// вывод
ostream& operator<<(ostream& output, Rational rational) {
    output << rational.Numerator() << "/"s << rational.Denominator();
    return output;
}

// ввод
istream& operator>>(istream& input, Rational& rational) {
    int numer, denominat;
    char drob;
    input >> numer >> drob >> denominat;
    rational = Rational{ numer, denominat };
    return input;
}

// сложение дробей
Rational operator+(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// вычитание дробей
Rational operator-(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// умножение дробей
Rational operator*(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Numerator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// унарный плюс
Rational operator+(Rational drob) {
    return drob;
}

// унарный минус
Rational operator-(Rational drob) {
    return { -drob.Numerator(),drob.Denominator()};
}

int main() {
    Rational rat();
}