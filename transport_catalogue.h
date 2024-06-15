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
		void Add(std::string, geo::Coordinates, std::unordered_map<std::string, double>);
		void Add(std::string_view, const std::vector<std::string_view>&, bool is_roundtrip);

		const Bus& GetBus(std::string_view) const;
		const Stop& GetStop(std::string_view) const;

		std::set<std::string_view> GetBusesByStop(std::string_view)const;
		const std::unordered_map<std::string_view, const Bus*> GetAllBuses()const;
		const std::map<std::string_view, const Bus*> GetAllBusesSorted()const;

		bool ContainsBus(std::string_view)const;
		bool ContainsStop(std::string_view)const;

		double ComputeStopsDistance(const std::vector<Stop*>&)const;
		double ComputeGeoDistance(const std::vector<Stop*>&) const;

		double GetDistanceBetweenStops(Stop*, Stop*)const;
		void SetDistanceBetweenStops(Stop*, Stop*, double)const;

		void Print();
	private:

		std::deque<Stop> stops_storage_;
		std::unordered_map<std::string_view, Stop*> stops_;

		std::deque<Bus> buses_storage_;
		std::unordered_map<std::string_view, Bus*> buses_;

		std::unordered_map<std::string_view, std::set<std::string_view>> buses_through_stop_;
	};

} // namespace TransportCatalogue