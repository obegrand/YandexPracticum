#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <memory>

template <typename T>
class RawMemory {
public:
	RawMemory() = default;

	explicit RawMemory(size_t capacity)
		: buffer_(Allocate(capacity))
		, capacity_(capacity) {
	}

	RawMemory(const RawMemory&) = delete;
	RawMemory& operator=(const RawMemory& rhs) = delete;

	RawMemory(RawMemory&& other) noexcept {
		buffer_ = other.buffer_;
		capacity_ = other.capacity_;
		other.buffer_ = nullptr;
		other.capacity_ = 0;
	}

	RawMemory& operator=(RawMemory&& rhs) noexcept {
		if (this != &rhs) {
			buffer_.~RawMemory();
			capacity_ = 0;
			Swap(rhs);
		}
		return *this;
	}

	~RawMemory() {
		Deallocate(buffer_);
	}

	T* operator+(size_t offset) noexcept {
		assert(offset <= capacity_);
		return buffer_ + offset;
	}

	const T* operator+(size_t offset) const noexcept {
		return const_cast<RawMemory&>(*this) + offset;
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<RawMemory&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < capacity_);
		return buffer_[index];
	}

	void Swap(RawMemory& other) noexcept {
		std::swap(buffer_, other.buffer_);
		std::swap(capacity_, other.capacity_);
	}

	const T* GetAddress() const noexcept {
		return buffer_;
	}

	T* GetAddress() noexcept {
		return buffer_;
	}

	size_t Capacity() const {
		return capacity_;
	}

private:
	// �������� ����� ������ ��� n ��������� � ���������� ��������� �� ��
	static T* Allocate(size_t n) {
		return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
	}

	// ����������� ����� ������, ���������� ����� �� ������ buf ��� ������ Allocate
	static void Deallocate(T* buf) noexcept {
		operator delete(buf);
	}

	T* buffer_ = nullptr;
	size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
	Vector() = default;

	explicit Vector(size_t size)
		: data_(size), size_(size) {
		std::uninitialized_value_construct_n(data_.GetAddress(), size);

	}

	Vector(const Vector& other)
		: data_(other.size_), size_(other.size_) {
		std::uninitialized_copy_n(other.data_.GetAddress(), other.size_, data_.GetAddress());
	}

	Vector(Vector&& other) noexcept {
		Swap(other);
	}

	~Vector() {
		std::destroy_n(data_.GetAddress(), size_);
	}

	Vector& operator=(const Vector& rhs) {
		if (this != &rhs) {
			if (rhs.size_ > data_.Capacity()) {
				/* ��������� copy-and-swap */
				Vector rhs_copy(rhs);
				Swap(rhs_copy);
			}
			else {
				// ����������� �������� �� rhs, ������ ��� ������������� ����� ��� ������ ������������
				if (rhs.size_ < size_) {
					std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + rhs.size_, data_.GetAddress());
					std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
				}
				else {
					std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + size_, data_.GetAddress());
					std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, data_.GetAddress() + size_);
				}
				size_ = rhs.size_;
			}
		}
		return *this;
	};

	Vector& operator=(Vector&& rhs) noexcept {
		if (this != &rhs) {
			Swap(rhs);
		}
		return *this;
	}

	void Swap(Vector& other) noexcept {
		data_.Swap(other.data_);
		std::swap(size_, other.size_);
	}

	size_t Size() const noexcept {
		return size_;
	}

	size_t Capacity() const noexcept {
		return data_.Capacity();
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<Vector&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < size_);
		return data_[index];
	}

	void Reserve(size_t new_capacity) {
		if (new_capacity <= data_.Capacity()) {
			return;
		}
		RawMemory<T> new_data(new_capacity);
		// constexpr �������� if ����� �������� �� ����� ����������
		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
			std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
		else
			std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
		std::destroy_n(data_.GetAddress(), size_);
		data_.Swap(new_data);
	}

	void Resize(size_t new_size) {
		if (new_size < size_) {
			std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
		}
		else {
			Reserve(new_size);
			std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
		}
		size_ = new_size;
	}

	void PushBack(const T& value) {
		if (size_ == Capacity()) {
			RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
			new (new_data + size_) T(value);
			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			else
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		else
			new (data_ + size_) T(value);
		++size_;
	}

	void PushBack(T&& value) {
		if (size_ == Capacity()) {
			RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
			new (new_data + size_) T(std::move(value));
			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			else
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		else
			new (data_ + size_) T(std::move(value));
		++size_;
	}

	void PopBack() noexcept {
		if (size_ > 0) {
			std::destroy_at(data_.GetAddress() + size_ - 1);
			--size_;
		}
	}

private:
	RawMemory<T> data_;
	size_t size_ = 0;
};