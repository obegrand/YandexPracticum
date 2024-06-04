#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class House {
public:
    House(int length, int width, int height) {
        specification_ = { length, width, height };
    }

    House(const HouseSpecification& spec) : specification_(spec) {}

    int GetLength() const { return specification_.length; }
    int GetWidth() const { return specification_.width; }
    int GetHeight() const { return specification_.height; }
private:
    HouseSpecification specification_;
};

class Resources {
public:
    Resources() = default;
    Resources(int brick_count) : brick_count_(brick_count){}

    void  TakeBricks(int count) {
        count < 0 || count > brick_count_ ?
             throw std::out_of_range("") : brick_count_ -= count;
    }

    int GetBrickCount() const {
        return brick_count_;
    }
private:
    int brick_count_ = 0;
};


class Builder {
public:
    Builder() = default;
    Builder(Resources& resources) : resources_(resources) {}

    House BuildHouse(HouseSpecification specification) {
        try
        {
            resources_.TakeBricks(specification.height * 8 * ((specification.length * 2) + (specification.width * 2)) * 4);
        }
        catch (std::out_of_range&)
        {
            throw std::runtime_error("");
        }
        return House{ specification };
    }
private:
    Resources& resources_;
};

int main() {
    Resources resources{ 10000 };
    Builder builder1{ resources };
    Builder builder2{ resources };

    House house1 = builder1.BuildHouse(HouseSpecification{ 12, 9, 3 });
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House house2 = builder2.BuildHouse(HouseSpecification{ 8, 6, 3 });
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House banya = builder1.BuildHouse(HouseSpecification{ 4, 3, 2 });
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;
}