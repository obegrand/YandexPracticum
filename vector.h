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
	explicit RawMemory(size_t capacity);
	RawMemory(RawMemory&& other) noexcept;

	RawMemory(const RawMemory&) = delete;
	RawMemory& operator=(const RawMemory& rhs) = delete;

	RawMemory& operator=(RawMemory&& rhs) noexcept;

	~RawMemory();

	T* operator+(size_t offset) noexcept;
	const T* operator+(size_t offset) const noexcept;

	T& operator[](size_t index) noexcept;
	const T& operator[](size_t index) const noexcept;

	const T* GetAddress() const noexcept;
	T* GetAddress() noexcept;

	void Swap(RawMemory& other) noexcept;
	size_t Capacity() const;

private:
	// Выделяет сырую память под n элементов и возвращает указатель на неё
	static T* Allocate(size_t n) {
		return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
	}

	// Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
	static void Deallocate(T* buf) noexcept {
		operator delete(buf);
	}

	T* buffer_ = nullptr;
	size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
	using iterator = T*;
	using const_iterator = const T*;

	Vector() = default;
	explicit Vector(size_t size);
	Vector(const Vector& other);
	Vector(Vector&& other) noexcept;
	~Vector();

	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;

	Vector& operator=(const Vector& rhs);
	Vector& operator=(Vector&& rhs) noexcept;

	T& operator[](size_t index) noexcept;
	const T& operator[](size_t index) const noexcept;

	size_t Size() const noexcept;
	size_t Capacity() const noexcept;

	void Swap(Vector& other) noexcept;

	void Reserve(size_t new_capacity);

	void Resize(size_t new_size);

	iterator Erase(const_iterator pos) /*noexcept(std::is_nothrow_move_assignable_v<T>)*/;

	iterator Insert(const_iterator pos, const T& value);
	iterator Insert(const_iterator pos, T&& value);

	void PushBack(const T& value);
	void PushBack(T&& value);

	void PopBack() noexcept;

	template <typename... Args>
	T& EmplaceBack(Args&&... args);

	template <typename... Args>
	iterator Emplace(const_iterator pos, Args&&... args);

private:
	RawMemory<T> data_;
	size_t size_ = 0;
};

template<typename T>
inline RawMemory<T>::RawMemory(size_t capacity)
	: buffer_(Allocate(capacity)), capacity_(capacity) {
}

template<typename T>
inline RawMemory<T>::RawMemory(RawMemory&& other) noexcept {
	buffer_ = other.buffer_;
	capacity_ = other.capacity_;
	other.buffer_ = nullptr;
	other.capacity_ = 0;
}

template<typename T>
inline RawMemory<T>& RawMemory<T>::operator=(RawMemory&& rhs) noexcept {
	if (this != &rhs) {
		Deallocate(buffer_);
		capacity_ = 0;
		Swap(rhs);
	}
	return *this;
}

template<typename T>
inline RawMemory<T>::~RawMemory() {
	Deallocate(buffer_);
}

template<typename T>
inline T* RawMemory<T>::operator+(size_t offset) noexcept {
	assert(offset <= capacity_);
	return buffer_ + offset;
}

template<typename T>
inline const T* RawMemory<T>::operator+(size_t offset) const noexcept {
	return const_cast<RawMemory&>(*this) + offset;
}

template<typename T>
inline const T& RawMemory<T>::operator[](size_t index) const noexcept {
	return const_cast<RawMemory&>(*this)[index];
}

template<typename T>
inline T& RawMemory<T>::operator[](size_t index) noexcept {
	assert(index < capacity_);
	return buffer_[index];
}

template<typename T>
inline void RawMemory<T>::Swap(RawMemory& other) noexcept {
	std::swap(buffer_, other.buffer_);
	std::swap(capacity_, other.capacity_);
}

template<typename T>
inline const T* RawMemory<T>::GetAddress() const noexcept {
	return buffer_;
}

template<typename T>
inline T* RawMemory<T>::GetAddress() noexcept {
	return buffer_;
}

template<typename T>
inline size_t RawMemory<T>::Capacity() const {
	return capacity_;
}

template<typename T>
inline Vector<T>::Vector(size_t size)
	: data_(size), size_(size) {
	std::uninitialized_value_construct_n(begin(), size);
}

template<typename T>
inline Vector<T>::Vector(const Vector& other)
	: data_(other.size_), size_(other.size_) {
	std::uninitialized_copy_n(other.data_.GetAddress(), other.size_, begin());
}

template<typename T>
inline Vector<T>::Vector(Vector&& other) noexcept {
	Swap(other);
}

template<typename T>
inline Vector<T>::~Vector() {
	std::destroy_n(begin(), size_);
}

template<typename T>
inline Vector<T>::iterator Vector<T>::begin() noexcept {
	return data_.GetAddress();
}

template<typename T>
inline Vector<T>::iterator Vector<T>::end() noexcept {
	return data_.GetAddress() + size_;
}

template<typename T>
inline Vector<T>::const_iterator Vector<T>::begin() const noexcept {
	return data_.GetAddress();
}

template<typename T>
inline Vector<T>::const_iterator Vector<T>::end() const noexcept {
	return data_.GetAddress() + size_;
}

template<typename T>
inline Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
	return begin();
}

template<typename T>
inline Vector<T>::const_iterator Vector<T>::cend() const noexcept {
	return end();
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector& rhs) {
	if (this != &rhs) {
		if (rhs.size_ > data_.Capacity()) {
			/* Применить copy-and-swap */
			Vector rhs_copy(rhs);
			Swap(rhs_copy);
		}
		else {
			// Скопировать элементы из rhs, создав при необходимости новые или удалив существующие
			if (rhs.size_ < size_) {
				std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + rhs.size_, begin());
				std::destroy_n(begin() + rhs.size_, size_ - rhs.size_);
			}
			else {
				std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + size_, begin());
				std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, begin() + size_);
			}
			size_ = rhs.size_;
		}
	}
	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept {
	if (this != &rhs) {
		Swap(rhs);
	}
	return *this;
}

template<typename T>
inline void Vector<T>::Swap(Vector& other) noexcept {
	data_.Swap(other.data_);
	std::swap(size_, other.size_);
}

template<typename T>
inline size_t Vector<T>::Size() const noexcept {
	return size_;
}

template<typename T>
inline size_t Vector<T>::Capacity() const noexcept {
	return data_.Capacity();
}

template<typename T>
inline const T& Vector<T>::operator[](size_t index) const noexcept {
	return const_cast<Vector&>(*this)[index];
}

template<typename T>
inline T& Vector<T>::operator[](size_t index) noexcept {
	assert(index < size_);
	return data_[index];
}

template<typename T>
inline void Vector<T>::Reserve(size_t new_capacity) {
	if (new_capacity <= data_.Capacity()) {
		return;
	}
	RawMemory<T> new_data(new_capacity);
	if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
		std::uninitialized_move_n(begin(), size_, new_data.GetAddress());
	else
		std::uninitialized_copy_n(begin(), size_, new_data.GetAddress());
	std::destroy_n(begin(), size_);
	data_.Swap(new_data);
}

template<typename T>
inline void Vector<T>::Resize(size_t new_size) {
	if (new_size < size_)
		std::destroy_n(begin() + new_size, size_ - new_size);
	else {
		Reserve(new_size);
		std::uninitialized_value_construct_n(begin() + size_, new_size - size_);
	}
	size_ = new_size;
}

template<typename T>
inline Vector<T>::iterator Vector<T>::Erase(const_iterator pos) {
	assert(pos >= begin() && pos < end());
	size_t offset = pos - begin();
	std::move(begin() + offset + 1, end(), begin() + offset);
	PopBack();
	return begin() + offset;
}

template<typename T>
inline Vector<T>::iterator Vector<T>::Insert(const_iterator pos, const T& value) {
	return Emplace(pos, value);
}

template<typename T>
inline Vector<T>::iterator Vector<T>::Insert(const_iterator pos, T&& value) {
	return Emplace(pos, std::move(value));
}

template<typename T>
inline void Vector<T>::PushBack(const T& value) {
	EmplaceBack(value);
}

template<typename T>
inline void Vector<T>::PushBack(T&& value) {
	EmplaceBack(std::move(value));
}

template<typename T>
inline void Vector<T>::PopBack() noexcept {
	if (size_ > 0) {
		std::destroy_at(data_.GetAddress() + size_ - 1);
		--size_;
	}
}

template<typename T>
template<typename ...Args>
inline Vector<T>::iterator Vector<T>::Emplace(const_iterator pos, Args && ...args) {
	assert(begin() <= pos && pos <= end());
	size_t offset = pos - begin();
	if (size_ == Capacity()) {
		RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
		new (new_data + offset) T(std::forward<Args>(args)...);

		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
			std::uninitialized_move_n(begin(), offset, new_data.GetAddress());
			std::uninitialized_move_n(begin() + offset, size_ - offset, new_data.GetAddress() + offset + 1);
		}
		else {
			std::uninitialized_copy_n(begin(), offset, new_data.GetAddress());
			std::uninitialized_copy_n(begin() + offset, size_ - offset, new_data.GetAddress() + offset + 1);
		}

		std::destroy_n(begin(), size_);
		data_.Swap(new_data);
	}
	else {
		if (size_ == offset)
			new (data_ + offset) T(std::forward<Args>(args)...);
		else {
			T temp(std::forward<Args>(args)...);
			new (end()) T(std::forward<T>(*(end() - 1)));
			std::move_backward(begin() + offset, end() - 1, end());
			data_[offset] = std::forward<T>(temp);
		}
	}
	++size_;
	return begin() + offset;
}

template<typename T>
template<typename ...Args>
inline T& Vector<T>::EmplaceBack(Args && ...args) {
	if (size_ == Capacity()) {
		RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
		new (new_data + size_) T(std::forward<Args>(args)...);

		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
			std::uninitialized_move_n(begin(), size_, new_data.GetAddress());
		else
			std::uninitialized_copy_n(begin(), size_, new_data.GetAddress());

		std::destroy_n(begin(), size_);
		data_.Swap(new_data);
	}
	else
		new (begin() + size_) T(std::forward<Args>(args)...);
	++size_;
	return *(end() - 1);
}