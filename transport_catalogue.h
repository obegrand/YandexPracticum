#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <vector>
#include <deque>

#include "domain.h"
#include <map>

namespace catalogue {
	class TransportCatalogue {
	public:
		void AddStop(std::string_view name, geo::Coordinates coordinates);
		void AddBus(std::string_view number, const std::vector<std::string_view>& stops, bool is_roundtrip);

		const Bus& GetBus(std::string_view number) const;
		const Stop& GetStop(std::string_view name) const;

		std::set<std::string_view> GetBusesByStop(const std::string_view name)const;
		const std::unordered_map<std::string_view, const Bus*> GetAllBuses()const;
		const std::map<std::string_view, const Bus*> GetAllBusesSorted()const;

		bool ContainsBus(std::string_view number)const;
		bool ContainsStop(std::string_view name)const;

		double ComputeStopsDistance(const std::vector<Stop*>& stop_names)const;
		double ComputeGeoDistance(const std::vector<Stop*>& stop_names) const;

		double GetDistanceBetweenStops(const Stop* stop1, const Stop* stop2)const;
		void SetDistanceBetweenStops(const Stop* stop1, const Stop* stop2, double distance);

		void SetDistances(std::string stop_name, const std::unordered_map<std::string, double>& distances);

		void Print();
	private:

		std::deque<Stop> stops_storage_;
		std::unordered_map<std::string_view, Stop*> stops_;

		std::deque<Bus> buses_storage_;
		std::unordered_map<std::string_view, Bus*> buses_;

		std::unordered_map<std::string_view, std::set<std::string_view>> buses_through_stop_;
	};

} // namespace TransportCatalogue