#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>

class ReserveProxyObj {
public:
	ReserveProxyObj(size_t capacity) : capacity_(capacity) { }

	size_t GetCapacity() const {
		return capacity_;
	}

private:
	size_t capacity_;
};

template <typename Type>
class SimpleVector {
public:
	using Iterator = Type*;
	using ConstIterator = const Type*;

	SimpleVector() noexcept = default;
	explicit SimpleVector(size_t size) : SimpleVector(size, Type()) { }
	SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size) {
		std::fill(items_.Get(), items_.Get() + size, value);
	}
	SimpleVector(std::initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size()) {
		std::copy(init.begin(), init.end(), items_.Get());
	}
	SimpleVector(const SimpleVector& other) : items_(other.capacity_), size_(other.size_) {
		std::copy(other.begin(), other.end(), items_.Get());
	}
	SimpleVector(SimpleVector&& other) : items_(other.capacity_) {
		swap(other);
	}
	explicit SimpleVector(ReserveProxyObj obj) {
		Reserve(obj.GetCapacity());
	}

	SimpleVector& operator=(const SimpleVector& rhs) {
		if (&items_ != &rhs.items_) {
			ArrayPtr<Type> temp(rhs.GetCapacity());
			std::move(rhs.begin(), rhs.end(), temp.Get());
			items_.swap(temp);
			size_ = rhs.GetSize();
			capacity_ = rhs.GetCapacity();
		}
		return *this;
	}

	size_t GetSize() const noexcept {
		return size_;
	}
	size_t GetCapacity() const noexcept {
		return capacity_;
	}
	bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	Type& operator[](size_t index) noexcept {
		return items_[index];
	}
	const Type& operator[](size_t index) const noexcept {
		return items_[index];
	}

	Type& At(size_t index) {
		if (index >= size_) throw std::out_of_range("out_of_range");
		return items_[index];
	}
	const Type& At(size_t index) const {
		if (index >= size_) throw std::out_of_range("out_of_range");
		return items_[index];
	}

	void Clear() noexcept {
		size_ = 0;
	}
	void Resize(size_t new_size) {
		if (new_size <= size_) {
			size_ = new_size;
		}
		if (new_size <= capacity_) {
			FillMove(items_.Get() + size_, items_.Get() + size_ + new_size);
		}
		if (new_size > capacity_) {
			size_t new_capacity = std::max(new_size, capacity_ * 2);
			ArrayPtr<Type> temp(new_capacity);
			FillMove(temp.Get(), temp.Get() + new_capacity);
			std::move(items_.Get(), items_.Get() + capacity_, temp.Get());
			items_.swap(temp);

			size_ = new_size;
			capacity_ = new_capacity;
		}
	}

	void PushBack(const Type& item) {
		if (size_ + 1 > capacity_) {
			size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
			ArrayPtr<Type> temp(new_capacity);
			std::fill(temp.Get(), temp.Get() + new_capacity, Type());
			std::copy(items_.Get(), items_.Get() + size_, temp.Get());
			items_.swap(temp);
			capacity_ = new_capacity;
		}
		items_[size_++] = item;
	}
	void PushBack(Type&& item) {
		if (size_ + 1 > capacity_) {
			size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
			ArrayPtr<Type> temp(new_capacity);
			std::move(items_.Get(), items_.Get() + size_, temp.Get());
			items_.swap(temp);
			capacity_ = new_capacity;
		}
		items_[size_++] = std::move(item);
	}

	Iterator Insert(ConstIterator pos, const Type& value) {
		assert(pos >= begin() && pos <= end());
		size_t count = pos - items_.Get();
		if (size_ == capacity_) {
			size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			ArrayPtr<Type> temp(new_capacity);
			std::copy(items_.Get(), items_.Get() + size_, temp.Get());
			items_.swap(temp);
			capacity_ = new_capacity;
		}
		std::copy_backward(items_.Get() + count, items_.Get() + size_, items_.Get() + size_ + 1);
		items_[count] = value;
		++size_;

		return &items_[count];
	}
	Iterator Insert(ConstIterator pos, Type&& value) {
		assert(pos >= begin() && pos <= end());
		size_t count = pos - items_.Get();
		if (size_ == capacity_) {
			size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			ArrayPtr<Type> temp(new_capacity);
			std::move(items_.Get(), items_.Get() + size_, temp.Get());
			items_.swap(temp);
			capacity_ = new_capacity;
		}
		for (size_t i = size_; i > count; --i) {
			items_[i] = std::move(items_[i - 1]);
		}		
		items_[count] = std::move(value);
		++size_;

		return &items_[count];
	}

	Iterator Erase(ConstIterator pos) {
		assert(pos != this->end());
		size_t count = pos - items_.Get();
		std::move(items_.Get() + count + 1, items_.Get() + size_, items_.Get() + count);
		--size_;

		return &items_[count];
	}

	void PopBack() noexcept {
		if (items_) --size_;
	}

	void swap(SimpleVector& other) noexcept {
		std::swap(capacity_, other.capacity_);
		std::swap(size_, other.size_);
		items_.swap(other.items_);
	}

	void Reserve(size_t new_capacity) {
		if (new_capacity > capacity_) {
			ArrayPtr<Type> temp(new_capacity);
			std::move(items_.Get(), items_.Get() + size_, temp.Get());
			items_.swap(temp);
			capacity_ = new_capacity;
		}
	}

	Iterator begin() noexcept {
		return items_.Get();
	}
	Iterator end() noexcept {
		return items_.Get() + size_;
	}
	ConstIterator begin() const noexcept {
		return items_.Get();
	}
	ConstIterator end() const noexcept {
		return items_.Get() + size_;
	}
	ConstIterator cbegin() const noexcept {
		return begin();
	}
	ConstIterator cend() const noexcept {
		return end();
	}
private:
	ArrayPtr<Type> items_;
	size_t size_ = 0;
	size_t capacity_ = 0;

	void FillMove(Iterator begin, Iterator end) {
		for (; begin != end; begin++) {
			*begin = std::move(Type());
		}
	}
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
	return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs < rhs);
}