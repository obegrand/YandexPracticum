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

	//добавление маршрута в базу,
	//добавление остановки в базу,
	//поиск маршрута по имени,
	//поиск остановки по имени,
	//получение информации о маршруте.

private:
	std::deque<Bus> buses_;
	std::deque<Stop> stops_;
};