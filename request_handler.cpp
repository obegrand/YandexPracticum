#include "request_handler.h"
#include <sstream>

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

RequestHandler::RequestHandler(JsonReader& requests, catalogue::TransportCatalogue& catalogue)
	: requests_(requests), db_(catalogue) {
	ProcessRequests();
}

void RequestHandler::ProcessRequests() const {
	if (requests_.GetStatRequests() == nullptr) return;
	json::Array result;
	const json::Array& stat_requests_arr = requests_.GetStatRequests().AsArray();
	for (const auto& stat_request : stat_requests_arr) {
		std::string type = stat_request.AsMap().at("type").AsString();
		if (type == "Stop") {
			result.push_back(PrintStop(stat_request.AsMap()));
		}
		else if (type == "Bus") {
			result.push_back(PrintBus(stat_request.AsMap()));
		}
		else if (type == "Map") {
			render::MapRender map_render(requests_.FillSettings(requests_.GetRenderSettings().AsMap()));
			result.push_back(PrintMap(stat_request.AsMap(),map_render));
		}
	}
	json::Print(json::Document{ result }, std::cout);
}

const json::Node RequestHandler::PrintBus(const json::Dict& request_map) const {
	json::Dict result;
	const std::string& bus_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!db_.ContainsBus(bus_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		catalogue::BusStat bus_stat = GetBusStat(bus_name).value();
		result["curvature"] = bus_stat.curvature;
		result["route_length"] = bus_stat.route_length;
		result["stop_count"] = bus_stat.stops_count;
		result["unique_stop_count"] = bus_stat.unique_stops_count;
	}
	return json::Node{ result };
}

const json::Node RequestHandler::PrintStop(const json::Dict& request_map) const {
	json::Dict result;
	const std::string& stop_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!db_.ContainsStop(stop_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		json::Array buses;
		for (auto& bus : GetBusesByStop(stop_name)) {
			buses.push_back(std::string(bus));
		}
		result["buses"] = buses;
	}
	return json::Node{ result };
}
const json::Node RequestHandler::PrintMap(const json::Dict& request_map, render::MapRender& map_render) const {
	json::Dict result;
	result["request_id"] = request_map.at("id").AsInt();
	std::stringstream output;
	map_render.GetMap(db_.GetAllBusesSorted()).Render(output);
	result["map"] = json::Node{ output.str() };

	return json::Node{ result };
}
std::optional<catalogue::BusStat> RequestHandler::GetBusStat(const std::string_view& bus_number) const {
	catalogue::BusStat bus_stat{};
	catalogue::Bus bus = db_.GetBus(bus_number);
	bus_stat.stops_count = bus.stop_names.size();
	std::set<std::string_view> countUniqueStops;
	for (auto stop : bus.stop_names) {
		countUniqueStops.insert(stop->name);
	}
	bus_stat.unique_stops_count = countUniqueStops.size();
	double distance = db_.ComputeStopsDistance(bus.stop_names);
	bus_stat.route_length = distance;
	bus_stat.curvature = distance / db_.ComputeGeoDistance(bus.stop_names);
	return bus_stat;
}

const std::set<std::string_view> RequestHandler::GetBusesByStop(std::string_view stop_name) const {
	return db_.GetBusesByStop(stop_name);
}