#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
	// Узел списка
	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value;
		Node* next_node = nullptr;
	};

	template <typename ValueType>
	class BasicIterator {
		friend class SingleLinkedList;

		explicit BasicIterator(Node* node) : node_(node) { }

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		BasicIterator() = default;

		BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) { }
		BasicIterator& operator=(const BasicIterator& rhs) = default;

		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}
		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return !(this->node_ == rhs.node_);
		}
		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}
		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return !(this->node_ == rhs.node_);
		}

		BasicIterator& operator++() noexcept {
			node_ = node_->next_node;
			return *this;
		}
		BasicIterator operator++(int) noexcept {
			auto old_value(*this);
			++(*this);
			return old_value;
		}

		[[nodiscard]] reference operator*() const noexcept {
			return node_->value;
		}
		[[nodiscard]] pointer operator->() const noexcept {
			return &node_->value;
		}
	private:
		Node* node_ = nullptr;
	};
public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;

	using Iterator = BasicIterator<Type>;
	using ConstIterator = BasicIterator<const Type>;

	[[nodiscard]] Iterator begin() noexcept {
		return Iterator{ head_.next_node };
	}
	[[nodiscard]] Iterator end() noexcept {
		return Iterator{ nullptr };
	}

	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator{ head_.next_node };
	}
	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator{ nullptr };
	}

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return begin();
	}
	[[nodiscard]] ConstIterator cend() const noexcept {
		return end();
	}

	SingleLinkedList() {
		head_.next_node = nullptr;
	}
	SingleLinkedList(std::initializer_list<Type> values) {
		for (Type value : values) {
			this->PushBack(value);
		}
	}
	SingleLinkedList(const SingleLinkedList& other) {
		assert(size_ == 0 && head_.next_node == nullptr);
		SingleLinkedList tmp;
		for (auto obj : other) {
			tmp.PushBack(obj);
		}
		swap(tmp);
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		SingleLinkedList tmp = rhs;
		this->swap(tmp);
		return *this;
	}
	~SingleLinkedList() {
		Clear();
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}

	void PushBack(const Type& value) {
		Node* newNode = new Node(value, nullptr);
		if (head_.next_node == nullptr) {
			head_.next_node = newNode;
		}
		else {
			Node* lastNode = head_.next_node;
			while (lastNode->next_node != nullptr) {
				lastNode = lastNode->next_node;
			}
			lastNode->next_node = newNode;
		}
		++size_;
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(other.head_.next_node, head_.next_node);
		std::swap(other.size_, size_);
	}

	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			Node* next_node = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = next_node;
			--size_;
		}
	}

	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}
	[[nodiscard]] bool IsEmpty() const noexcept {
		return size_ == 0;
	}

private:
	Node head_;
	size_t size_ = 0;
};

void PrintSingleLinkedList(SingleLinkedList<int>& list) {
	for (auto node : list) {
		std::cout << node << " ";
	}
	std::cout << std::endl;
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs.GetSize() != rhs.GetSize() || !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs < rhs || lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs > rhs || lhs == rhs);
}

// Эта функция проверяет работу класса SingleLinkedList
void Test3() {
	// Проверка списков на равенство и неравенство
	{
		SingleLinkedList<int> list_1;
		list_1.PushFront(1);
		list_1.PushFront(2);

		SingleLinkedList<int> list_2;
		list_2.PushFront(1);
		list_2.PushFront(2);
		list_2.PushFront(3);

		SingleLinkedList<int> list_1_copy;
		list_1_copy.PushFront(1);
		list_1_copy.PushFront(2);

		SingleLinkedList<int> empty_list;
		SingleLinkedList<int> another_empty_list;

		// Список равен самому себе
		assert(list_1 == list_1);
		assert(empty_list == empty_list);

		// Списки с одинаковым содержимым равны, а с разным - не равны
		assert(list_1 == list_1_copy);
		assert(list_1 != list_2);
		assert(list_2 != list_1);
		assert(empty_list == another_empty_list);
	}

	// Обмен содержимого списков
	{
		SingleLinkedList<int> first;
		first.PushFront(1);
		first.PushFront(2);

		SingleLinkedList<int> second;
		second.PushFront(10);
		second.PushFront(11);
		second.PushFront(15);

		const auto old_first_begin = first.begin();
		const auto old_second_begin = second.begin();
		const auto old_first_size = first.GetSize();
		const auto old_second_size = second.GetSize();

		first.swap(second);

		assert(second.begin() == old_first_begin);
		assert(first.begin() == old_second_begin);
		assert(second.GetSize() == old_first_size);
		assert(first.GetSize() == old_second_size);

		// Обмен при помощи функции swap
		{
			using std::swap;

			// В отсутствие пользовательской перегрузки будет вызвана функция std::swap, которая
			// выполнит обмен через создание временной копии
			swap(first, second);

			// Убеждаемся, что используется не std::swap, а пользовательская перегрузка

			// Если бы обмен был выполнен с созданием временной копии,
			// то итератор first.begin() не будет равен ранее сохранённому значению,
			// так как копия будет хранить свои узлы по иным адресам
			assert(first.begin() == old_first_begin);
			assert(second.begin() == old_second_begin);
			assert(first.GetSize() == old_first_size);
			assert(second.GetSize() == old_second_size);
		}
	}

	// Инициализация списка при помощи std::initializer_list
	{
		SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
		assert(list.GetSize() == 5);
		assert(!list.IsEmpty());
		assert(std::equal(list.begin(), list.end(), std::begin({ 1, 2, 3, 4, 5 })));
	}

	// Лексикографическое сравнение списков
	{
		using IntList = SingleLinkedList<int>;

		assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
		assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
	}

	// Копирование списков
	{
		const SingleLinkedList<int> empty_list{};
		// Копирование пустого списка
		{
			auto list_copy(empty_list);
			assert(list_copy.IsEmpty());
		}

		SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
		// Копирование непустого списка
		{
			auto list_copy(non_empty_list);

			assert(non_empty_list.begin() != list_copy.begin());
			assert(list_copy == non_empty_list);
		}
	}

	// Присваивание списков
	{
		const SingleLinkedList<int> source_list{ 1, 2, 3, 4 };

		SingleLinkedList<int> receiver{ 5, 4, 3, 2, 1 };
		receiver = source_list;
		assert(receiver.begin() != source_list.begin());
		assert(receiver == source_list);
	}

	// Вспомогательный класс, бросающий исключение после создания N-копии
	struct ThrowOnCopy {
		ThrowOnCopy() = default;
		explicit ThrowOnCopy(int& copy_counter) noexcept
			: countdown_ptr(&copy_counter) {
		}
		ThrowOnCopy(const ThrowOnCopy& other)
			: countdown_ptr(other.countdown_ptr)  //
		{
			if (countdown_ptr) {
				if (*countdown_ptr == 0) {
					throw std::bad_alloc();
				}
				else {
					--(*countdown_ptr);
				}
			}
		}
		// Присваивание элементов этого типа не требуется
		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
		// Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
		// Как только обнулится, конструктор копирования выбросит исключение
		int* countdown_ptr = nullptr;
	};

	// Безопасное присваивание списков
	{
		SingleLinkedList<ThrowOnCopy> src_list;
		src_list.PushFront(ThrowOnCopy{});
		src_list.PushFront(ThrowOnCopy{});
		auto thrower = src_list.begin();
		src_list.PushFront(ThrowOnCopy{});

		int copy_counter = 0;  // при первом же копировании будет выброшено исключение
		thrower->countdown_ptr = &copy_counter;

		SingleLinkedList<ThrowOnCopy> dst_list;
		dst_list.PushFront(ThrowOnCopy{});
		int dst_counter = 10;
		dst_list.begin()->countdown_ptr = &dst_counter;
		dst_list.PushFront(ThrowOnCopy{});

		try {
			dst_list = src_list;
			// Ожидается исключение при присваивании
			assert(false);
		}
		catch (const std::bad_alloc&) {
			// Проверяем, что состояние списка-приёмника не изменилось
			// при выбрасывании исключений
			assert(dst_list.GetSize() == 2);
			auto it = dst_list.begin();
			assert(it != dst_list.end());
			assert(it->countdown_ptr == nullptr);
			++it;
			assert(it != dst_list.end());
			assert(it->countdown_ptr == &dst_counter);
			assert(dst_counter == 10);
		}
		catch (...) {
			// Других типов исключений не ожидается
			assert(false);
		}
	}
}

int main() {
	Test3();
}