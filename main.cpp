#include <iostream>
#include <algorithm> // Убедитесь, что эта строка не закомментирована
#include <numeric>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end)
		: first(begin)
		, last(end) {
	}

	Iterator begin() const {
		return first;
	}

	Iterator end() const {
		return last;
	}

private:
	const Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
	return IteratorRange{ v.begin(), next(v.begin(), min(top, v.size())) };
}

struct Person {
	string name;
	int age, income;
	bool is_male;
};

static vector<Person> ReadPeople(istream& input) {
	int count;
	input >> count;

	vector<Person> result(count);
	for (Person& p : result) {
		char gender;
		input >> p.name >> p.age >> p.income >> gender;
		p.is_male = gender == 'M';
	}

	return result;
}

std::pair<std::string, std::string> most_popular_name(const std::vector<Person>& people) {
	std::map<std::string_view, int> male_name_count;
	std::map<std::string_view, int> female_name_count;

	for (const Person& p : people) {
		if (p.is_male) {
			++male_name_count[p.name];
		}
		else {
			++female_name_count[p.name];
		}
	}

	std::string most_popular_male_name;
	std::string most_popular_female_name;
	int max_male_count = 0;
	int max_female_count = 0;

	for (const auto& [name, count] : male_name_count) {
		if (count > max_male_count) {
			max_male_count = count;
			most_popular_male_name = name;
		}
	}

	for (const auto& [name, count] : female_name_count) {
		if (count > max_female_count) {
			max_female_count = count;
			most_popular_female_name = name;
		}
	}

	return { most_popular_male_name, most_popular_female_name };
}

vector<uint64_t> PeopleByWealthy(std::vector<Person>& people) {
	vector<uint64_t> result; 
	result.reserve(people.size());
	sort(people.begin(), people.end(),
		[](const Person& lhs, const Person& rhs) {
			return lhs.income > rhs.income;
		});
	uint64_t cur_sum = 0;
	for (const Person& person : people) {
		cur_sum += person.income;
		result.push_back(cur_sum);
	}
	return result;
}

int main() {
	vector<Person> people = ReadPeople(cin);
	auto [most_popular_male_name, most_popular_female_name] = most_popular_name(people);
	vector<uint64_t> people_by_wealthy = PeopleByWealthy(people);
	int adult_age, count;
	char gender;

	sort(people.begin(), people.end(),
		[](const Person& lhs, const Person& rhs) {
			return lhs.age < rhs.age;
		});

	for (string command; cin >> command;)
		if (command == "AGE"s) {
			cin >> adult_age;

			auto adult_begin = lower_bound(people.begin(), people.end(), adult_age,
				[](const Person& lhs, int age) {
					return lhs.age < age;
				});

			cout << "There are "s << distance(adult_begin, people.end()) << " adult people for maturity age "s
				<< adult_age << '\n';
		}
		else if (command == "WEALTHY"s) {
			cin >> count;

			cout << "Top-"s << count << " people have total income "s << people_by_wealthy[count - 1] << '\n';
		}
		else if (command == "POPULAR_NAME"s) {
			cin >> gender;

			if (gender == 'M') {
				cout << "Most popular name among people of gender "s << gender << " is "s << most_popular_male_name << '\n';
			}
			else {
				cout << "Most popular name among people of gender "s << gender << " is "s << most_popular_female_name << '\n';
			}
		}
}