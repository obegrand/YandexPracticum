#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) :size_(a_size), capacity_(N) {
        if (size_ > capacity_)throw std::invalid_argument("invalid_argument");
    }

    T& operator[](size_t index) {
        return array_[index];
    }
    const T& operator[](size_t index) const {
        return array_[index];
    }

    std::array<T, N>::iterator begin() {
        return array_.begin();
    }
    std::array<T, N>::iterator end() {
        return array_.begin() + size_;
    }
    std::array<T, N>::const_iterator begin() const {
        return array_.begin();
    }
    std::array<T, N>::const_iterator end() const {
        return array_.begin() + size_;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const T& value) {
        if (size_ >= capacity_)throw std::overflow_error("overflow_error");
        array_[size_++] = value;
    }
    T PopBack() {
        if (size_ < 1)throw std::underflow_error("underflow_error");
        return array_[--size_];
    }
private:
    std::array<T, N> array_;
    size_t size_;
    size_t capacity_;
};