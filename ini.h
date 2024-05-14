#include <iostream>
#include <string>
#include <unordered_map>

// ��������� ��� ����� ���������� INI � ������������ ��� ini

using Section = std::unordered_map<std::string, std::string>;

class Document {
public:
    // ���������� ������� ������ ���� � ����� ini.cpp
    Section& AddSection(std::string name);
    const Section& GetSection(const std::string& name) const;
    std::size_t GetSectionCount() const;

private:
    std::unordered_map<std::string, Section> sections_;
};

// ����������� ���� ������� ������ ���� � ����� ini.cpp
Document Load(std::istream& input);