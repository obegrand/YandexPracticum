#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <vector>

#include "geo.h"

namespace TransportCatalogue {

struct Stop {
	std::string name;
	Geo::Coordinates coordinates;
};

struct Bus {
	std::string number;
	std::vector<std::string> stops;
};

class TransportCatalogue {
public:
	void Add(const Stop& stop);
	void Add(const Bus& bus);

	const Bus GetBus(const std::string_view number)const;
	const Stop GetStop(const std::string_view name)const;
	std::set<std::string> GetBusesByStop(const std::string_view name)const;

	bool ContainsBus(const std::string_view number)const;
	bool ContainsStop(const std::string_view number)const;

	double ComputeBusDistance(const Bus& bus)const;

private:
	std::unordered_map<std::string, Bus> buses_;
	std::unordered_map<std::string, Stop> stops_;
	std::unordered_map<std::string, std::set<std::string>> buses_by_stop_;
};

} // namespace TransportCatalogue