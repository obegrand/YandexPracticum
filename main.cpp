// Макрос _USE_MATH_DEFINES необходим, чтобы при подключении <cmath> была объявлена константа M_PI
#define _USE_MATH_DEFINES
// Макрос _USE_MATH_DEFINES следует объявить ДО подключения других заголовочных файлов,
// которые могут подключить <cmath> неявно
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

using namespace std;

enum class Color { RED, GREEN, BLUE };

ostream& operator<<(ostream& out, Color color) {
	switch (color) {
	case Color::RED:
		out << "red"s;
		break;
	case Color::GREEN:
		out << "green"s;
		break;
	case Color::BLUE:
		out << "blue"s;
		break;
	}
	return out;
}

class Shape
{
public:
	Shape(Color color) : color_(color) {}

	Color GetColor() const {
		return color_;
	}

	void SetColor(Color color) {
		color_ = color;
	}

	virtual double GetArea() const {
		return area_;
	}

	virtual std::string GetType() const {
		return type_;
	}

private:
	Color color_;
	double area_ = 0.0;
	std::string type_ = "Shape";
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height, Color color) : Shape(color), width_(width), height_(height) {}

	std::string GetType() const override {
		return type_;
	}

	double GetWidth() const {
		return width_;
	}

	double GetHeight() const {
		return height_;
	}

	void SetSize(double width, double height) {
		width_ = width;
		height_ = height;
	}

	double GetArea() const override {
		return width_ * height_;
	}
private:
	double width_;
	double height_;
	std::string type_ = "Rectangle";
};

class Circle : public Shape {
public:
	Circle(double radius, Color color) : Shape(color), radius_(radius) {}

	std::string GetType() const override {
		return type_;
	}

	double GetRadius()const {
		return radius_;
	}

	void SetRadius(const double radius) {
		radius_ = radius;
	}

	double GetArea() const override {
		return M_PI * radius_ * radius_; // Используйте M_PI для числа π
	}
private:
	std::string type_ = "Circle";
	double radius_;
};

template <typename ShapeCollection>
double CalcSumArea(const ShapeCollection& collection) {
	return accumulate(begin(collection), end(collection), 0.0, [](double sum, const Shape* shape) {
		return sum + shape->GetArea();
		});
}

void PrintShapeInfo(const Shape& shape) {
	cout << shape.GetType() << ": color: "s << shape.GetColor() << ", area:"s << shape.GetArea() << endl;
}

int main() {
	Circle c{ 10.0, Color::RED };
	Rectangle r{ 10, 20, Color::BLUE };
	Shape sh{ Color::GREEN };

	const Shape* shapes[] = { &c, &r, &sh };

	assert(sh.GetType() == "Shape"s);
	assert(c.GetType() == "Circle"s);
	assert(r.GetType() == "Rectangle"s);

	assert(sh.GetColor() == Color::GREEN);
	assert(c.GetColor() == Color::RED);
	assert(r.GetColor() == Color::BLUE);
	sh.SetColor(Color::BLUE);
	c.SetColor(Color::GREEN);
	r.SetColor(Color::RED);
	assert(sh.GetColor() == Color::BLUE);
	assert(c.GetColor() == Color::GREEN);
	assert(r.GetColor() == Color::RED);

	assert(std::abs(r.GetArea() - 200.0) < 1e-5);
	assert(std::abs(c.GetArea() - 314.15) < 1e-2);
	c.SetRadius(1.0);
	assert(std::abs(c.GetArea() - 3.1415) < 1e-4);
	r.SetSize(5, 7);
	assert(r.GetWidth() == 5);
	assert(r.GetHeight() == 7);
	assert(std::abs(r.GetArea() - 35.0) < 1e-5);

	assert(abs(CalcSumArea(shapes) - 38.1416) < 1e-4);

	for (const Shape* shape : shapes) {
		PrintShapeInfo(*shape);
	}
	cout << "Total area: " << CalcSumArea(shapes) << endl;
}