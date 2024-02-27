#include <numeric>
#include <ostream>
#include <vector>
#include <iostream>

using namespace std;

// ����� ����� � ���� *numerator* / *denominator*
class Rational {
public:
    // �������� ����� 0/1
    Rational() = default;

    // �������� ����� *numerator*/1 
    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    // �������� ����� *numerator* / *denominator*
    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    // ��������� *numerator*
    int Numerator() const {
        return numerator_;
    }

    // ��������� *denominator*
    int Denominator() const {
        return denominator_;
    }
private:
    // ���������� � ���(����������� ������ ��������)
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

// �����
ostream& operator<<(ostream& output, Rational rational) {
    output << rational.Numerator() << "/"s << rational.Denominator();
    return output;
}

// ����
istream& operator>>(istream& input, Rational& rational) {
    int numer, denominat;
    char drob;
    input >> numer >> drob >> denominat;
    rational = Rational{ numer, denominat };
    return input;
}

// �������� ������
Rational operator+(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// ��������� ������
Rational operator-(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// ��������� ������
Rational operator*(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Numerator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

// ������� ����
Rational operator+(Rational drob) {
    return drob;
}

// ������� �����
Rational operator-(Rational drob) {
    return { -drob.Numerator(),drob.Denominator()};
}

int main() {
    Rational rat();
}