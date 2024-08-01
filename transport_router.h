#pragma once
#include "router.h"
#include "transport_catalogue.h"
#include <memory>

namespace transport {

struct RoutingSettings {
	int bus_wait_time_ = 0;
	double bus_velocity_ = 0.0;
};

class Router{
public:
	Router(RoutingSettings settings, const catalogue::TransportCatalogue& catalogue)
		:bus_wait_time_(settings.bus_wait_time_), bus_velocity_(settings.bus_velocity_) {
		BuildGraph(catalogue);
	}

	const graph::DirectedWeightedGraph<double>& BuildGraph(const catalogue::TransportCatalogue& catalogue);
	const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;
	const graph::DirectedWeightedGraph<double>& GetGraph() const;
	const std::string& GetStopName(const graph::VertexId stop_name) const;
	int GetWaitTime() const;

private:
	int bus_wait_time_ = 0;
	double bus_velocity_ = 0.0;

	graph::DirectedWeightedGraph<double> graph_;
	std::unordered_map<std::string, graph::VertexId> stop_ids_;
	std::unordered_map<graph::VertexId, std::string> stop_ids_reverse_;
	std::unique_ptr<graph::Router<double>> router_;
};
}