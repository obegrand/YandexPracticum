#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
	// ���� ������
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

	SingleLinkedList() {
		head_.next_node = nullptr;
	}
	SingleLinkedList(const SingleLinkedList& other) {
		assert(size_ == 0 && head_.next_node == nullptr);
		SingleLinkedList tmp;
		for (auto obj : other) {
			tmp.PushBack(obj);
		}
		swap(tmp);
	}
	SingleLinkedList(std::initializer_list<Type> values) {
		for (Type value : values) {
			this->PushBack(value);
		}
	}
	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		SingleLinkedList tmp = rhs;
		this->swap(tmp);
		return *this;
	}
	~SingleLinkedList() {
		Clear();
	}

	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator{ &head_ };
	}
	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator{ const_cast<Node*>(&head_) };
	}
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return before_begin();
	}

	[[nodiscard]] Iterator begin() noexcept {
		return Iterator{ head_.next_node };
	}
	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator{ head_.next_node };
	}
	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return begin();
	}

	[[nodiscard]] Iterator end() noexcept {
		return Iterator{ nullptr };
	}
	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator{ nullptr };
	}
	[[nodiscard]] ConstIterator cend() const noexcept {
		return end();
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

	void PopFront() noexcept {
		auto next_node = head_.next_node->next_node;
		delete head_.next_node;
		head_.next_node = next_node;
		--size_;
	}

	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		assert(pos.node_ != nullptr);

		pos.node_->next_node = new Node(value, pos.node_->next_node);
		++size_;
		return Iterator{ pos.node_->next_node };
	}

	Iterator EraseAfter(ConstIterator pos) noexcept {
		assert(!IsEmpty());
		assert(pos.node_ != nullptr);

		auto next_node = pos.node_->next_node->next_node;
		delete pos.node_->next_node;
		pos.node_->next_node = next_node;
		--size_;
		return Iterator{ pos.node_->next_node };
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(other.head_.next_node, this->head_.next_node);
		std::swap(other.size_, this->size_);
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

// ��� ������� ��������� ������ SingleLinkedList
void Test0() {
	using namespace std;
	{
		const SingleLinkedList<int> empty_int_list;
		assert(empty_int_list.GetSize() == 0u);
		assert(empty_int_list.IsEmpty());
	}

	{
		const SingleLinkedList<string> empty_string_list;
		assert(empty_string_list.GetSize() == 0u);
		assert(empty_string_list.IsEmpty());
	}
}

void Test1() {
	// �����, �������� �� ����� ���������
	struct DeletionSpy {
		DeletionSpy() = default;
		explicit DeletionSpy(int& instance_counter) noexcept
			: instance_counter_ptr_(&instance_counter)  //
		{
			OnAddInstance();
		}
		DeletionSpy(const DeletionSpy& other) noexcept
			: instance_counter_ptr_(other.instance_counter_ptr_)  //
		{
			OnAddInstance();
		}
		DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
			if (this != &rhs) {
				auto rhs_copy(rhs);
				std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
			}
			return *this;
		}
		~DeletionSpy() {
			OnDeleteInstance();
		}

	private:
		void OnAddInstance() noexcept {
			if (instance_counter_ptr_) {
				++(*instance_counter_ptr_);
			}
		}
		void OnDeleteInstance() noexcept {
			if (instance_counter_ptr_) {
				assert(*instance_counter_ptr_ != 0);
				--(*instance_counter_ptr_);
			}
		}

		int* instance_counter_ptr_ = nullptr;
	};

	// �������� ������� � ������
	{
		SingleLinkedList<int> l;
		assert(l.IsEmpty());
		assert(l.GetSize() == 0u);

		l.PushFront(0);
		l.PushFront(1);
		assert(l.GetSize() == 2);
		assert(!l.IsEmpty());

		l.Clear();
		assert(l.GetSize() == 0);
		assert(l.IsEmpty());
	}

	// �������� ������������ �������� ���������
	{
		int item0_counter = 0;
		int item1_counter = 0;
		int item2_counter = 0;
		{
			SingleLinkedList<DeletionSpy> list;
			list.PushFront(DeletionSpy{ item0_counter });
			list.PushFront(DeletionSpy{ item1_counter });
			list.PushFront(DeletionSpy{ item2_counter });

			assert(item0_counter == 1);
			assert(item1_counter == 1);
			assert(item2_counter == 1);
			list.Clear();
			assert(item0_counter == 0);
			assert(item1_counter == 0);
			assert(item2_counter == 0);

			list.PushFront(DeletionSpy{ item0_counter });
			list.PushFront(DeletionSpy{ item1_counter });
			list.PushFront(DeletionSpy{ item2_counter });
			assert(item0_counter == 1);
			assert(item1_counter == 1);
			assert(item2_counter == 1);
		}
		assert(item0_counter == 0);
		assert(item1_counter == 0);
		assert(item2_counter == 0);
	}

	// ��������������� �����, ��������� ���������� ����� �������� N-�����
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
		// ������������ ��������� ����� ���� �� ���������
		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
		// ����� �������� ��������� �������. ���� �� ����� nullptr, �� ����������� ��� ������ �����������.
		// ��� ������ ���������, ����������� ����������� �������� ����������
		int* countdown_ptr = nullptr;
	};

	{
		bool exception_was_thrown = false;
		// ��������������� ��������� ������� ����������� �� ����, ���� �� ����� ��������� ����������
		for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
			// ������ �������� ������
			SingleLinkedList<ThrowOnCopy> list;
			list.PushFront(ThrowOnCopy{});
			try {
				int copy_counter = max_copy_counter;
				list.PushFront(ThrowOnCopy(copy_counter));
				// ���� ����� �� �������� ����������, ������ ������ ������� � ����� ���������
				assert(list.GetSize() == 2);
			}
			catch (const std::bad_alloc&) {
				exception_was_thrown = true;
				// ����� ������������ ���������� ��������� ������ ������ �������� �������
				assert(list.GetSize() == 1);
				break;
			}
		}
		assert(exception_was_thrown);
	}
}

void Test2() {
	// ������������ �� ������� ������
	{
		SingleLinkedList<int> list;
		// ����������� ������ ��� ������� � ����������� ������� begin()/end()
		const auto& const_list = list;

		// ��������� begin � end � ������� ��������� ����� ���� �����
		assert(list.begin() == list.end());
		assert(const_list.begin() == const_list.end());
		assert(list.cbegin() == list.cend());
		assert(list.cbegin() == const_list.begin());
		assert(list.cend() == const_list.end());
	}

	// ������������ �� ��������� ������
	{
		SingleLinkedList<int> list;
		const auto& const_list = list;

		list.PushFront(1);
		assert(list.GetSize() == 1u);
		assert(!list.IsEmpty());

		assert(const_list.begin() != const_list.end());
		assert(const_list.cbegin() != const_list.cend());
		assert(list.begin() != list.end());

		assert(const_list.begin() == const_list.cbegin());

		assert(*list.cbegin() == 1);
		*list.begin() = -1;
		assert(*list.cbegin() == -1);

		const auto old_begin = list.cbegin();
		list.PushFront(2);
		assert(list.GetSize() == 2);

		const auto new_begin = list.cbegin();
		assert(new_begin != old_begin);
		// �������� ��������������
		{
			auto new_begin_copy(new_begin);
			assert((++(new_begin_copy)) == old_begin);
		}
		// �������� ��������������
		{
			auto new_begin_copy(new_begin);
			assert(((new_begin_copy)++) == new_begin);
			assert(new_begin_copy == old_begin);
		}
		// ��������, ����������� �� ������� ����� ���������� ��������, ����� ��������� end()
		{
			auto old_begin_copy(old_begin);
			assert((++old_begin_copy) == list.end());
		}
	}
	// �������������� ����������
	{
		SingleLinkedList<int> list;
		list.PushFront(1);
		// ��������������� ConstIterator �� Iterator
		SingleLinkedList<int>::ConstIterator const_it(list.begin());
		assert(const_it == list.cbegin());
		assert(*const_it == *list.cbegin());

		SingleLinkedList<int>::ConstIterator const_it1;
		// ������������ ConstIterator'� �������� Iterator
		const_it1 = list.begin();
		assert(const_it1 == const_it);
	}
	// �������� ��������� ->
	{
		using namespace std;
		SingleLinkedList<std::string> string_list;

		string_list.PushFront("one"s);
		assert(string_list.cbegin()->length() == 3u);
		string_list.begin()->push_back('!');
		assert(*string_list.begin() == "one!"s);
	}
}

void Test3() {
	// �������� ������� �� ��������� � �����������
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

		// ������ ����� ������ ����
		assert(list_1 == list_1);
		assert(empty_list == empty_list);

		// ������ � ���������� ���������� �����, � � ������ - �� �����
		assert(list_1 == list_1_copy);
		assert(list_1 != list_2);
		assert(list_2 != list_1);
		assert(empty_list == another_empty_list);
	}

	// ����� ����������� �������
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

		// ����� ��� ������ ������� swap
		{
			using std::swap;

			// � ���������� ���������������� ���������� ����� ������� ������� std::swap, �������
			// �������� ����� ����� �������� ��������� �����
			swap(first, second);

			// ����������, ��� ������������ �� std::swap, � ���������������� ����������

			// ���� �� ����� ��� �������� � ��������� ��������� �����,
			// �� �������� first.begin() �� ����� ����� ����� ����������� ��������,
			// ��� ��� ����� ����� ������� ���� ���� �� ���� �������
			assert(first.begin() == old_first_begin);
			assert(second.begin() == old_second_begin);
			assert(first.GetSize() == old_first_size);
			assert(second.GetSize() == old_second_size);
		}
	}

	// ������������� ������ ��� ������ std::initializer_list
	{
		SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
		assert(list.GetSize() == 5);
		assert(!list.IsEmpty());
		assert(std::equal(list.begin(), list.end(), std::begin({ 1, 2, 3, 4, 5 })));
	}

	// ������������������ ��������� �������
	{
		using IntList = SingleLinkedList<int>;

		assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
		assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
	}

	// ����������� �������
	{
		const SingleLinkedList<int> empty_list{};
		// ����������� ������� ������
		{
			auto list_copy(empty_list);
			assert(list_copy.IsEmpty());
		}

		SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
		// ����������� ��������� ������
		{
			auto list_copy(non_empty_list);

			assert(non_empty_list.begin() != list_copy.begin());
			assert(list_copy == non_empty_list);
		}
	}

	// ������������ �������
	{
		const SingleLinkedList<int> source_list{ 1, 2, 3, 4 };

		SingleLinkedList<int> receiver{ 5, 4, 3, 2, 1 };
		receiver = source_list;
		assert(receiver.begin() != source_list.begin());
		assert(receiver == source_list);
	}

	// ��������������� �����, ��������� ���������� ����� �������� N-�����
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
		// ������������ ��������� ����� ���� �� ���������
		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
		// ����� �������� ��������� �������. ���� �� ����� nullptr, �� ����������� ��� ������ �����������.
		// ��� ������ ���������, ����������� ����������� �������� ����������
		int* countdown_ptr = nullptr;
	};

	// ���������� ������������ �������
	{
		SingleLinkedList<ThrowOnCopy> src_list;
		src_list.PushFront(ThrowOnCopy{});
		src_list.PushFront(ThrowOnCopy{});
		auto thrower = src_list.begin();
		src_list.PushFront(ThrowOnCopy{});

		int copy_counter = 0;  // ��� ������ �� ����������� ����� ��������� ����������
		thrower->countdown_ptr = &copy_counter;

		SingleLinkedList<ThrowOnCopy> dst_list;
		dst_list.PushFront(ThrowOnCopy{});
		int dst_counter = 10;
		dst_list.begin()->countdown_ptr = &dst_counter;
		dst_list.PushFront(ThrowOnCopy{});

		try {
			dst_list = src_list;
			// ��������� ���������� ��� ������������
			assert(false);
		}
		catch (const std::bad_alloc&) {
			// ���������, ��� ��������� ������-�������� �� ����������
			// ��� ������������ ����������
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
			// ������ ����� ���������� �� ���������
			assert(false);
		}
	}
}

void Test4() {
	struct DeletionSpy {
		~DeletionSpy() {
			if (deletion_counter_ptr) {
				++(*deletion_counter_ptr);
			}
		}
		int* deletion_counter_ptr = nullptr;
	};

	// �������� PopFront
	{
		SingleLinkedList<int> numbers{ 3, 14, 15, 92, 6 };
		numbers.PopFront();
		assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

		SingleLinkedList<DeletionSpy> list;
		list.PushFront(DeletionSpy{});
		int deletion_counter = 0;
		list.begin()->deletion_counter_ptr = &deletion_counter;
		assert(deletion_counter == 0);
		list.PopFront();
		assert(deletion_counter == 1);
	}

	// ������ � �������, �������������� begin
	{
		SingleLinkedList<int> empty_list;
		const auto& const_empty_list = empty_list;
		assert(empty_list.before_begin() == empty_list.cbefore_begin());
		assert(++empty_list.before_begin() == empty_list.begin());
		assert(++empty_list.cbefore_begin() == const_empty_list.begin());

		SingleLinkedList<int> numbers{ 1, 2, 3, 4 };
		const auto& const_numbers = numbers;
		assert(numbers.before_begin() == numbers.cbefore_begin());
		assert(++numbers.before_begin() == numbers.begin());
		assert(++numbers.cbefore_begin() == const_numbers.begin());
	}

	// ������� �������� ����� ��������� �������
	{  // ������� � ������ ������
		{
			SingleLinkedList<int> lst;
			const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
			assert((lst == SingleLinkedList<int>{123}));
			assert(inserted_item_pos == lst.begin());
			assert(*inserted_item_pos == 123);
		}

		// ������� � �������� ������
		{
			SingleLinkedList<int> lst{ 1, 2, 3 };
			auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

			assert(inserted_item_pos == lst.begin());
			assert(inserted_item_pos != lst.end());
			assert(*inserted_item_pos == 123);
			assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

			inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
			assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
			assert(*inserted_item_pos == 555);
			assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
		};
	}

	// ��������������� �����, ��������� ���������� ����� �������� N-�����
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
		// ������������ ��������� ����� ���� �� ���������
		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
		// ����� �������� ��������� �������. ���� �� ����� nullptr, �� ����������� ��� ������ �����������.
		// ��� ������ ���������, ����������� ����������� �������� ����������
		int* countdown_ptr = nullptr;
	};

	// �������� ����������� ������� �������� ������������ ����������
	{
		bool exception_was_thrown = false;
		for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
			SingleLinkedList<ThrowOnCopy> list{ ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{} };
			try {
				int copy_counter = max_copy_counter;
				list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
				assert(list.GetSize() == 4u);
			}
			catch (const std::bad_alloc&) {
				exception_was_thrown = true;
				assert(list.GetSize() == 3u);
				break;
			}
		}
		assert(exception_was_thrown);
	}

	// �������� ��������� ����� ��������� �������
	{
		{
			SingleLinkedList<int> lst{ 1, 2, 3, 4 };
			const auto& const_lst = lst;
			const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
			assert((lst == SingleLinkedList<int>{2, 3, 4}));
			assert(item_after_erased == lst.begin());
		}
		{
			SingleLinkedList<int> lst{ 1, 2, 3, 4 };
			const auto item_after_erased = lst.EraseAfter(lst.cbegin());
			assert((lst == SingleLinkedList<int>{1, 3, 4}));
			assert(item_after_erased == (++lst.begin()));
		}
		{
			SingleLinkedList<int> lst{ 1, 2, 3, 4 };
			const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
			assert((lst == SingleLinkedList<int>{1, 2, 3}));
			assert(item_after_erased == lst.end());
		}
		{
			SingleLinkedList<DeletionSpy> list{ DeletionSpy{}, DeletionSpy{}, DeletionSpy{} };
			auto after_begin = ++list.begin();
			int deletion_counter = 0;
			after_begin->deletion_counter_ptr = &deletion_counter;
			assert(deletion_counter == 0u);
			list.EraseAfter(list.cbegin());
			assert(deletion_counter == 1u);
		}
	}
}

int main() {
	Test0();
	Test1();
	Test2();
	Test3();
	Test4();
}