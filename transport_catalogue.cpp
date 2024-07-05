#include "transport_catalogue.h"
#include <string_view>
#include <stdexcept>
#include <iostream>

namespace catalogue {

	void TransportCatalogue::AddStop(std::string_view name, geo::Coordinates coordinates) {
		stops_storage_.push_back({ std::string(name), coordinates, {} });

		stops_[stops_storage_.back().name] = &stops_storage_.back();
		buses_through_stop_[stops_storage_.back().name];
	}

	void TransportCatalogue::AddBus(std::string_view number, const std::vector<std::string_view>& stops, bool is_roundtrip) {
		Bus newBus = { .number = std::string(number),.stop_names = {},.is_roundtrip = is_roundtrip };
		for (const auto& stop : stops) {
			auto it = stops_.find(stop);
			if (it != stops_.end()) {
				newBus.stop_names.push_back(it->second);
			}
		}

		buses_storage_.push_back(std::move(newBus));
		buses_[buses_storage_.back().number] = &buses_storage_.back();

		for (const auto& stop : stops) {
			buses_through_stop_[stop].insert(buses_storage_.back().number);
		}

	}

	const Bus& TransportCatalogue::GetBus(std::string_view number) const {
		return buses_.contains(number) ? *buses_.at(number) : throw std::runtime_error("Bus not found");
	}

	const Stop& TransportCatalogue::GetStop(std::string_view name) const {
		return stops_.contains(name) ? *stops_.at(name) : throw std::runtime_error("Stop not found");
	}

	std::set<std::string_view> TransportCatalogue::GetBusesByStop(const std::string_view name) const {
		return buses_through_stop_.at(name);
	}

	const std::unordered_map<std::string_view, const Bus*> TransportCatalogue::GetAllBuses() const {
		std::unordered_map<std::string_view, const Bus*> result;
		for (const auto& bus : buses_) {
			result.emplace(bus);
		}
		return result;
	}

	const std::map<std::string_view, const Bus*> TransportCatalogue::GetAllBusesSorted() const {
		std::map<std::string_view, const Bus*> result;
		for (const auto& bus : buses_) {
			result.emplace(bus);
		}
		return result;
	}

	bool TransportCatalogue::ContainsBus(std::string_view number)const {
		return buses_.contains(number);
	}

	bool TransportCatalogue::ContainsStop(std::string_view name)const {
		return stops_.contains(name);
	}

	double TransportCatalogue::ComputeStopsDistance(const std::vector<Stop*>& stop_names) const {
		double result = 0.0;
		for (size_t i = 1; i < stop_names.size(); i++) {
			result += GetDistanceBetweenStops(stop_names.at(i - 1), stop_names.at(i));
		}
		return result;
	}

	double TransportCatalogue::GetDistanceBetweenStops(const Stop* stop1, const Stop* stop2) const {
		if (stop1->distances_to_other_stops.contains(stop2->name)) {
			return stop1->distances_to_other_stops.find(stop2->name)->second;
		}
		else if (stop2->distances_to_other_stops.contains(stop1->name)) {
			return stop2->distances_to_other_stops.find(stop1->name)->second;
		}
		else return 0;
	}

	void TransportCatalogue::SetDistanceBetweenStops(const Stop* stop1, const Stop* stop2, double distance) {
		auto it = stops_.find(stop1->name);
		if (it != stops_.end()) { 
			it->second->distances_to_other_stops[stop2->name] = distance;
		};
	}

	void TransportCatalogue::Print() {
		std::cerr << "\nStops:" << std::endl;
		for (auto& stop : stops_storage_) {
			std::cerr << "  " << stop.name << ": " <<
				stop.coordinates.lat << "," << stop.coordinates.lng << std::endl;
			for (auto& stop_name : stop.distances_to_other_stops) {
				std::cerr << "    " << stop_name.first << ": " << stop_name.second << std::endl;
			}
		}
		std::cerr << "Buses:" << std::endl;
		for (auto& bus : buses_storage_) {
			std::cerr << "  " << bus.number << ": " << bus.is_roundtrip << std::endl;
			for (auto& stop_name : bus.stop_names) {
				std::cerr << "    " << stop_name->name << std::endl;
			}
		}
	}

	double TransportCatalogue::ComputeGeoDistance(const std::vector<Stop*>& stop_names) const {
		double result = 0.0;

		for (size_t i = 1; i < stop_names.size(); i++) {
			geo::Coordinates from = stop_names[i - 1]->coordinates;
			geo::Coordinates to = stop_names[i]->coordinates;

			result += geo::ComputeDistance(std::move(from), std::move(to));
		}
		return result;
	}

} // namespace TransportCatalogue