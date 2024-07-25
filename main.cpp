#include <algorithm>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class Domain {
public:
	// конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
	Domain(const std::string& domain_name) : domain_(domain_name) {
		std::reverse(domain_.begin(), domain_.end());
	}

	// разработайте operator==
	bool operator==(const Domain& other) const {
		return domain_ == other.domain_;
	}

	//существует для работы компараторов
	bool operator<(const Domain& other) const {
		return domain_ < other.domain_;
	}

	// разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
	bool IsSubdomain(const Domain& subdomain) const {
		if (subdomain.domain_.size() <= domain_.size() + 1) return false;

		if (subdomain.domain_[domain_.size()] == '.') {
			std::string_view sub(subdomain.domain_.begin(), subdomain.domain_.begin() + domain_.size());
			return domain_ == sub;
		}
		else return false;
	}

	std::string GetDomain() const {
		return domain_;
	}

private:
	std::string domain_;
};

class DomainChecker {
public:
	// конструктор должен принимать список запрещённых доменов через пару итераторов
	template <typename Iterator>
	DomainChecker(Iterator first, Iterator last) : ban_domains_(first, last) {
		std::ranges::sort(ban_domains_, [](const Domain& lhs, const Domain& rhs) {
			return lhs < rhs;
			});
		last = std::unique(ban_domains_.begin(), ban_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
			return rhs == lhs || lhs.IsSubdomain(rhs);
			});
		ban_domains_.erase(last, ban_domains_.end());
	}

	// разработайте метод IsForbidden, возвращающий true, если домен запрещён
	bool IsForbidden(const Domain& subdomain) const {
		auto it = std::upper_bound(ban_domains_.begin(), ban_domains_.end(), subdomain, [](const Domain& lhs, const Domain& rhs) {
			return lhs < rhs;
			});
		if (it == ban_domains_.begin()) return false;
		else return subdomain == *(it - 1) || (it - 1)->IsSubdomain(subdomain);
	}
private:
	std::vector<Domain> ban_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
const std::vector<Domain> ReadDomains(std::istream& input, size_t number) {
	std::vector<Domain> result;
	result.reserve(number);
	std::string line;
	for (size_t i = 0; i < number; ++i) {
		getline(input, line);
		result.push_back(line);
	}
	return result;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input) {
	std::string line;
	getline(input, line);

	Number num;
	std::istringstream(line) >> num;

	return num;
}

int main() {
	using namespace std::string_view_literals;
	const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
	DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

	const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
	for (const Domain& domain : test_domains) {
		std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
	}
}