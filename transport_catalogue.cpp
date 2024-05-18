#include "transport_catalogue.h"
#include <string_view>

void TransportCatalogue::Add(Stop stop) {
	stops_.push_back(stop);
    buses_by_stop_[stop.name];
}

void TransportCatalogue::Add(Bus bus) {
	buses_.push_back(bus);
    for (const auto& stop : bus.stops ) {
        buses_by_stop_[stop].insert(bus.number);
    }
}

Bus TransportCatalogue::GetBus(std::string_view number) const {
    for (const auto& bus : buses_) {
        if (bus.number == number) {
            return bus;
        }
    }
    throw std::runtime_error("Bus not found");
}

std::set<std::string> TransportCatalogue::GetBuses(const std::string_view name) const {
    return buses_by_stop_.at(std::string(name));
}

Stop TransportCatalogue::GetStop(const std::string_view name) const {
    for (const auto& stop : stops_) {
        if (stop.name == name) {
            return stop;
        }
    }
    throw std::runtime_error("Stop not found");
}

bool TransportCatalogue::ContainsBus(std::string_view number) const {
    for (const auto& bus : buses_) {
        if (bus.number == number) {
            return true;
        }
    }
    return false;
}

bool TransportCatalogue::ContainsStop(const std::string_view name) const{
    return buses_by_stop_.count(std::string(name));
}

double TransportCatalogue::ComputeBusDistance(Bus& bus) const {
    double result = 0.0;
    Stop from, to;
    for (size_t i = 1; i < bus.stops.size(); i++) {
            from = GetStop(bus.stops[i - 1]);
            to = GetStop(bus.stops[i]);

            result += ComputeDistance(from.coordinates, to.coordinates);
        }
    return result;
}

