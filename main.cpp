#include <array>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

//class ParkingCounter {
//public:
//    void Park(VehiclePlate car) {
//        ++car_to_parks_[car];
//    }
// 
//    int GetCount(const VehiclePlate& car) const {
//        if (!car_to_parks_.contains(car)) 
//            return {};
//        return car_to_parks_.at(car);
//    }
//
//    auto& GetAllData() const {
//        return car_to_parks_;
//    }
//
//private:
//    unordered_map<VehiclePlate, int, VehiclePlateHasher> car_to_parks_;
//};

class VehiclePlate {
private:
    auto AsTuple() const {
        return tie(letters_, digits_, region_);
    }

public:
    bool operator==(const VehiclePlate& other) const {
        return AsTuple() == other.AsTuple();
    }

    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{ l0, l1, l2 }
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }
private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

class VehiclePlateHasher {
public:
    size_t operator()(const VehiclePlate& plate) const {
        return static_cast<size_t>(hasher_(plate.ToString()));
    }
private:
    hash<string> hasher_;
};

struct ParkingException {};

template <typename Clock>
class Parking {
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;

public:
    Parking(int cost_per_second) : cost_per_second_(cost_per_second) {}

    // запарковать машину с указанным номером
    void Park(VehiclePlate car) {
        if (now_parked_.contains(car)) throw ParkingException();

        now_parked_[car] = Clock::now();
    }

    // забрать машину с указанным номером
    void Withdraw(const VehiclePlate& car) {
        if (!now_parked_.contains(car)) throw ParkingException();

        TimePoint end = Clock::now();
        complete_parks_[car] = end - now_parked_[car];
        now_parked_.erase(car);
    }

    // получить счёт за конкретный автомобиль
    int64_t GetCurrentBill(const VehiclePlate& car) const {
        int64_t result = 0;

        if (!now_parked_.contains(car) && !complete_parks_.contains(car))
            return result;
        if (now_parked_.contains(car)) {
            result += chrono::duration_cast<chrono::seconds>(Clock::now() - now_parked_.at(car)).count() * cost_per_second_;
        }
        if (complete_parks_.contains(car)) {
            result += chrono::duration_cast<chrono::seconds>(complete_parks_.at(car)).count() * cost_per_second_;
        }

        return result;
    }

    // завершить расчётный период
    // те машины, которые находятся на парковке на данный момент, должны 
    // остаться на парковке, но отсчёт времени для них начинается с нуля
    unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> EndPeriodAndGetBills() {
        unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> result;

        for (auto [car, start_parking] : now_parked_) {
            Duration duration = Clock::now() - now_parked_.at(car);
            result[car] += chrono::duration_cast<chrono::seconds>(duration).count() * cost_per_second_;
            now_parked_[car] = Clock::now();
        }
        for (auto [car, duration] : complete_parks_) {
            result[car] += chrono::duration_cast<chrono::seconds>(duration).count() * cost_per_second_;
        }
        complete_parks_.clear();

        return result;
    }

    // не меняйте этот метод
    auto& GetNowParked() const {
        return now_parked_;
    }

    // не меняйте этот метод
    auto& GetCompleteParks() const {
        return complete_parks_;
    }

private:
    int cost_per_second_;
    unordered_map<VehiclePlate, TimePoint, VehiclePlateHasher> now_parked_;
    unordered_map<VehiclePlate, Duration, VehiclePlateHasher> complete_parks_;
};

// эти часы удобно использовать для тестирования
// они покажут столько времени, сколько вы задали явно
class TestClock {
public:
    using time_point = chrono::system_clock::time_point;
    using duration = chrono::system_clock::duration;

    static void SetNow(int seconds) {
        current_time_ = seconds;
    }

    static time_point now() {
        return start_point_ + chrono::seconds(current_time_);
    }

private:
    inline static time_point start_point_ = chrono::system_clock::now();
    inline static int current_time_ = 0;
};

int main() {
    Parking<TestClock> parking(10);

    TestClock::SetNow(10);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(20);
    parking.Withdraw({ 'A', 'A', 111, 'A', 99 });
    parking.Park({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(40);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 100);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 200);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(50);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 200);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 300);
    assert(parking.GetCurrentBill({ 'C', 'C', 333, 'C', 99 }) == 0);
    parking.Withdraw({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(70);
    {
        // проверим счёт
        auto bill = parking.EndPeriodAndGetBills();

        // так как внутри макроса используется запятая,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
            == unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                {{'A', 'A', 111, 'A', 99}, 400},
                { {'B', 'B', 222, 'B', 99}, 300 },
        }));
    }

    TestClock::SetNow(80);
    {
        // проверим счёт
        auto bill = parking.EndPeriodAndGetBills();

        // так как внутри макроса используется запятая,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
            == unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                {{'A', 'A', 111, 'A', 99}, 100},
        }));
    }

    try {
        parking.Park({ 'A', 'A', 111, 'A', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    try {
        parking.Withdraw({ 'B', 'B', 222, 'B', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    cout << "Success!"s << endl;
}