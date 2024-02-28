#include <numeric>
#include <ostream>
#include <iostream>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    Rational operator+=(Rational drob) {
        numerator_ = numerator_ * drob.Denominator() + drob.Numerator() * denominator_;
        denominator_ *= drob.Denominator();
        Normalize();
        return *this;
    }
    
    Rational operator-=(Rational drob) {
        numerator_ = numerator_ * drob.Denominator() - drob.Numerator() * denominator_;
        denominator_ *= drob.Denominator();
        Normalize();
        return *this;
    }
        
    Rational operator*=(Rational drob) {
        numerator_ *= drob.Numerator();
        denominator_ *= drob.Denominator();
        Normalize();
        return *this;
    }
            
    Rational operator/=(Rational drob) {
        numerator_ *= drob.Denominator();
        denominator_ *= drob.Numerator();
        Normalize();
        return *this;
    }
private:
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

bool operator==(Rational left, Rational right) {
    return left.Numerator() == right.Numerator() && left.Denominator() == right.Denominator();
}

bool operator!=(Rational left, Rational right) {
    return !(left == right);
}

bool operator<(Rational left, Rational right) {
    return left.Numerator() / left.Denominator() < right.Numerator() / right.Denominator();
}

bool operator>(Rational left, Rational right) {
    return !(left < right);
}

bool operator<=(Rational left, Rational right) {
    return (left < right) || (left == right);
}

bool operator>=(Rational left, Rational right) {
    return !(left < right) || (left == right);
}

ostream& operator<<(ostream& output, Rational rational) {
    output << rational.Numerator() << "/"s << rational.Denominator();
    return output;
}

istream& operator>>(istream& input, Rational& rational) {
    int numer, denominat;
    char drob;
    input >> numer >> drob >> denominat;
    rational = Rational{ numer, denominat };
    return input;
}

Rational operator+(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

Rational operator-(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

Rational operator*(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Numerator();
    const int denominator = left.Denominator() * right.Denominator();
    return { numerator, denominator };
}

Rational operator/(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator();
    const int denominator = left.Denominator() * right.Numerator();
    return { numerator, denominator };
}

Rational operator+(Rational drob) {
    return drob;
}

Rational operator-(Rational drob) {
    return { -drob.Numerator(),drob.Denominator()};
}

int main() {
    while (true)
    {
        Rational a, b;
        cin >> a >> b;

        // Аналогично if (b != Rational{0})
        if (a < b) {
            cout << a << "<" << b << endl;
        }
        else {
            cout << a << ">" << b << endl;
        }

    }
}