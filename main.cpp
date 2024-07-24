#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <algorithm>

class Domain {
public:
	// ����������� ������ ��������� ��������������� �� string, � ���������� ������������ ����
	Domain(const std::string& domain_name) : main_domain_(domain_name) {}

	// ������������ operator==
	bool operator==(const Domain& other) const {
		return main_domain_ == other.main_domain_;
	}

	bool operator<(const Domain& other) const {
		return main_domain_ < other.main_domain_;
	}

	// ������������ ����� IsSubdomain, ����������� ������ ����� � ������������ true, ���� this ��� ��������
	bool IsSubdomain(const Domain& potential_subdomain) const {
		if (main_domain_ == potential_subdomain.main_domain_) return true;
		else if (potential_subdomain.main_domain_.size() <= main_domain_.size() + 1) return false;
		size_t dot_pos = potential_subdomain.main_domain_.size() - main_domain_.size() - 1;

		if (potential_subdomain.main_domain_[dot_pos] == '.') {
			return potential_subdomain.main_domain_.substr(dot_pos + 1) == main_domain_;
		} 
		else return false;
	}

	std::string GetDomain() const {
		return main_domain_;
	}

private:
	std::string main_domain_;
};


class DomainChecker {
public:
	// ����������� ������ ��������� ������ ����������� ������� ����� ���� ����������
	template <typename Iterator>
	DomainChecker(Iterator first, Iterator last) : forbidden_domains_(first, last) {
		//���� ���-�� ������������� � ������� ����������� � ��������� �������
	}

	// ������������ ����� IsForbidden, ������������ true, ���� ����� ��������
	bool IsForbidden(const Domain& domain) const {
		//���� �������� ����� �������� ������� �� ������ �� ���������� � ��������
		for (const Domain& dom : forbidden_domains_) {
			if (dom.IsSubdomain(domain)) return true;
		}
		return false;
	}
private:
	std::vector<Domain> forbidden_domains_;
};

// ������������ ������� ReadDomains, �������� �������� ���������� ������� �� ������������ �����
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