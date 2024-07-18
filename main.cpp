#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <numeric>

class Ebook_sim {
public:
	void Read(size_t user_id, uint16_t page_count) {
		if (users_read_progression_.contains(user_id))
			page_count_by_user_[users_read_progression_[user_id]]--; // delete outdated data

		page_count_by_user_[page_count]++;
		users_read_progression_[std::move(user_id)] = std::move(page_count);
	}

	float Cheer(size_t user_id) {
		size_t total_users = users_read_progression_.size();

		if (!users_read_progression_.contains(user_id)) return 0; // user not found
		if (total_users == 1) return 1; // hi single user

		size_t less_than_user = std::accumulate(page_count_by_user_.begin(),
			page_count_by_user_.lower_bound(users_read_progression_[std::move(user_id)]), 0,
			[](size_t sum, const std::pair<uint16_t, size_t>& p) {
				return sum + p.second;
			});

		return static_cast<float>(less_than_user) / (total_users - 1);
	}

private:
	std::unordered_map<size_t, uint16_t> users_read_progression_; // <user_id, read_progress>
	std::map<uint16_t, size_t> page_count_by_user_; // <read_progress, how many users on this stage>
};


int main() {
	size_t query_size;
	std::cin >> query_size;
	Ebook_sim e_book;

	for (size_t i = 0; i < query_size; i++) {
		std::string query_type;
		std::cin >> query_type;
		if (query_type == "READ") {
			size_t user_id;
			uint16_t page_count;
			std::cin >> user_id >> page_count;
			e_book.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			size_t user_id;
			std::cin >> user_id;
			std::cout << std::setprecision(6) << e_book.Cheer(user_id) << std::endl;
		}
		else throw std::logic_error("invalid command");
	}
	return 0;
}