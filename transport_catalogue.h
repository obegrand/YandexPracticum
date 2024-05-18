#pragma once
#include <deque>
#include <string>
#include <list>
#include <set>
#include <unordered_set>
#include <vector> // Add this line
#include <map>

#include "geo.h"
#include <optional>

struct Stop {
	std::string name;
	Coordinates coordinates;
};

struct Bus {
	std::string number;
	std::vector<std::string> stops;
	bool circular_route;
};

class TransportCatalogue {
public:
	void Add(Stop stop);

	void Add(Bus bus);

	Bus GetBus(const std::string_view number) const;
	std::set<std::string> GetBuses(const std::string_view name) const;
	Stop GetStop(const std::string_view name) const;

	bool ContainsBus(const std::string_view number) const;
	bool ContainsStop(const std::string_view number) const;

	double ComputeBusDistance(Bus& bus) const;

private:
	std::deque<Bus> buses_;
	std::deque<Stop> stops_;
	std::map<std::string, std::set<std::string>> buses_by_stop_;
};