#pragma once
#include <unordered_set>
#include <unordered_map>
#include <deque>

class TransportCatalogue{

	struct Bus {
		
	};

	struct Stop {

	};

public:

	//���������� �������� � ����,
	//���������� ��������� � ����,
	//����� �������� �� �����,
	//����� ��������� �� �����,
	//��������� ���������� � ��������.

private:
	std::deque<Bus> buses_;
	std::deque<Stop> stops_;
};