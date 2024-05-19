#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

namespace InputCommand {

struct CommandDescription {
	// ����������, ������ �� ������� (���� command ��������)
	explicit operator bool() const {
		return !command.empty();
	}

	bool operator!() const {
		return !operator bool();
	}

	std::string command;      // �������� �������
	std::string id;           // id �������� ��� ���������
	std::string description;  // ��������� �������
};

class InputReader {
public:
	// ������ ������ � ��������� CommandDescription � ��������� ��������� � commands_
	void ParseLine(std::string_view line);

	// ��������� ������� ������������ ����������, ��������� ������� �� commands_
	void ApplyCommands(TransportCatalogue::TransportCatalogue& catalogue) const;

private:
	std::vector<CommandDescription> commands_;
};

} // namespace inputCommand