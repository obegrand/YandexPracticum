#pragma once
#include <chrono>
#include <iostream>
#include <string>

class LogDuration {
public:
    LogDuration(const std::string name) 
        : name_(name)
    { }

    ~LogDuration() {
        const auto end_time = std::chrono::steady_clock::now();
        const auto dur = end_time - start_time_;
        std::cerr << name_ << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << std::endl;
    }

private:
    // В переменной будет время конструирования объекта LogDuration
    const std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();
    const std::string name_;
};