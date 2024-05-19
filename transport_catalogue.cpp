#include "transport_catalogue.h"
#include <string_view>

namespace TransportCatalogue {

void TransportCatalogue::Add(const Stop& stop) {
	stops_[stop.name] = stop;
	buses_by_stop_[stop.name];
}

void TransportCatalogue::Add(const Bus& bus) {
	buses_[bus.number] = bus;
	for (const auto& stop : bus.stops) {
		buses_by_stop_[stop].insert(bus.number);
	}
}

const Bus TransportCatalogue::GetBus(std::string_view number)const {
	return buses_.at(std::string(number));
}

const Stop TransportCatalogue::GetStop(const std::string_view name)const {
	return stops_.at(std::string(name));
}

std::set<std::string> TransportCatalogue::GetBusesByStop(const std::string_view name) const {
	return buses_by_stop_.at(std::string(name));
}

bool TransportCatalogue::ContainsBus(const std::string_view number)const {
	return buses_.contains(std::string(number));
}

bool TransportCatalogue::ContainsStop(const std::string_view name)const {
	return buses_by_stop_.count(std::string(name));
}

double TransportCatalogue::ComputeBusDistance(const Bus& bus) const {
	double result = 0.0;
	for (size_t i = 1; i < bus.stops.size(); i++) {
		Geo::Coordinates from = stops_.at(bus.stops[i - 1]).coordinates;
		Geo::Coordinates to = stops_.at(bus.stops[i]).coordinates;

		result += Geo::ComputeDistance(std::move(from), std::move(to));
	}
	return result;
}

} // namespace TransportCatalogue