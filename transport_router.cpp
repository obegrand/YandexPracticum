#include "transport_router.h"

namespace transport {

	const graph::DirectedWeightedGraph<double>& transport::Router::BuildGraph(const catalogue::TransportCatalogue& catalogue) {
		const auto& all_stops = catalogue.GetAllStops();
		const auto& all_buses = catalogue.GetAllBuses();

		graph::DirectedWeightedGraph<double> stops_graph(all_stops.size());

		{
			graph::VertexId vertex_id = 0;

			for (const auto& [stop_name, stop] : all_stops) {
				stop_ids_reverse_[vertex_id] = stop->name;
				stop_ids_[stop->name] = vertex_id++;
			}
		}

		for (const auto& [bus_name, bus] : all_buses) {
			const std::vector<catalogue::Stop*>& stops = bus->stop_names;
			for (size_t from_id = 0; from_id < stops.size(); ++from_id) {
				for (size_t to_id = from_id + 1; to_id < stops.size(); ++to_id) {
					double distance = 0;
					double distance_reverse = 0;
					
					const catalogue::Stop* stop_from = stops[from_id];
					const catalogue::Stop* stop_to = stops[to_id];
					
					for (size_t from_to_count = from_id + 1; from_to_count <= to_id; ++from_to_count) {
						distance += catalogue.GetDistanceBetweenStops(stops[from_to_count - 1], stops[from_to_count]);
						distance_reverse += catalogue.GetDistanceBetweenStops(stops[from_to_count], stops[from_to_count - 1]);
					}
					stops_graph.AddEdge({
							/*from*/	stop_ids_.at(stop_from->name),
							/*to*/		stop_ids_.at(stop_to->name),
							/*distance*/static_cast<double>(distance) / (bus_velocity_ * (100.0 / 6.0)) + static_cast<double>(bus_wait_time_),
							/*name*/	bus->number,
							/*stops_count*/	to_id - from_id
						});

					if (!bus->is_roundtrip) {
						stops_graph.AddEdge({
							/*from*/	stop_ids_.at(stop_to->name),
							/*to*/		stop_ids_.at(stop_from->name),
							/*distance*/static_cast<double>(distance_reverse) / (bus_velocity_ * (100.0 / 6.0)) + static_cast<double>(bus_wait_time_),
							/*name*/	bus->number,
							/*stops_count*/	to_id - from_id
						});
					}
				}
			}
		}

		graph_ = std::move(stops_graph);
		router_ = std::make_unique<graph::Router<double>>(graph_);

		return graph_;
	}

	const std::optional<graph::Router<double>::RouteInfo> Router::FindRoute(const std::string_view stop_from, const std::string_view stop_to) const {
		return router_->BuildRoute(stop_ids_.at(std::string(stop_from)), stop_ids_.at(std::string(stop_to)));
	}

	const graph::DirectedWeightedGraph<double>& Router::GetGraph() const {
		return graph_;
	}

	const std::string& Router::GetStopName(const graph::VertexId stop_name) const {
		return stop_ids_reverse_.at(stop_name);
	}
	int Router::GetWaitTime() const {
		return bus_wait_time_;
	}
}