#pragma once

#include <cassert>
#include <cstdlib>
#include <algorithm>

template <typename Type>
class ArrayPtr {
public:
    // �������������� ArrayPtr ������� ����������
    ArrayPtr() = default;

    // ������ � ���� ������ �� size ��������� ���� Type.
    // ���� size == 0, ���� raw_ptr_ ������ ���� ����� nullptr
    explicit ArrayPtr(size_t size) {
        if (size == 0) raw_ptr_ = nullptr;
        else raw_ptr_ = new Type[size];
    }

    // ����������� �� ������ ���������, ��������� ����� ������� � ���� ���� nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept
        : raw_ptr_(raw_ptr)
    {
    }

    // ��������� �����������
    ArrayPtr(const ArrayPtr&) = delete;

    // ����������� �����������
    ArrayPtr(ArrayPtr&& other) noexcept
        : raw_ptr_(other.raw_ptr_) {
        other.raw_ptr_ = nullptr;
    }

    // �������� ������������ ������������
    ArrayPtr& operator=(ArrayPtr&& other) noexcept {
        if (this != &other) {
            delete[] raw_ptr_; // ���������� ������� ������
            raw_ptr_ = other.raw_ptr_; // ����������� ���������
            other.raw_ptr_ = nullptr; // �������� ��������� � ������������ �������
        }
        return *this;
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    // ��������� ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // ���������� ��������� �������� � ������, ���������� �������� ������ �������
    // ����� ������ ������ ��������� �� ������ ������ ����������
    [[nodiscard]] Type* Release() noexcept {
        Type* tmp = raw_ptr_;
        raw_ptr_ = nullptr;
        return tmp;
    }

    // ���������� ������ �� ������� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    // ���������� ����������� ������ �� ������� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        return raw_ptr_[index];
    }

    // ���������� true, ���� ��������� ���������, � false � ��������� ������
    explicit operator bool() const {
        if (raw_ptr_) return true;
        return false;
    }

    // ���������� �������� ������ ���������, ��������� ����� ������ �������
    Type* Get() const noexcept {
        return &raw_ptr_[0];
    }

    // ������������ ��������� ��������� �� ������ � �������� other
    void swap(ArrayPtr& other) noexcept {
        std::swap(other.raw_ptr_, raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};