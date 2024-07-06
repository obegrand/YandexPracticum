#include "request_handler.h"
#include <sstream>
#include "json_builder.h"

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
	for (const auto& stat_request : requests_.GetStatRequests().AsArray()) {
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
	json::Node result;
	const std::string& bus_name = request_map.at("name").AsString();
	const int id = request_map.at("id").AsInt();
	if (!db_.ContainsBus(bus_name)) {
		result = json::Builder{}
				.StartDict()
					.Key("request_id").Value(id)
					.Key("error_message").Value("not found")
				.EndDict()
			.Build();
	}
	else {
		catalogue::BusStat bus_stat = GetBusStat(bus_name).value();
		result = json::Builder{}
				.StartDict()
					.Key("curvature").Value(bus_stat.curvature)
					.Key("route_length").Value(bus_stat.route_length)
					.Key("stop_count").Value(bus_stat.stops_count)
					.Key("unique_stop_count").Value(bus_stat.unique_stops_count)
					.Key("request_id").Value(id)
				.EndDict()
			.Build();
	}
	return result;
}

const json::Node RequestHandler::PrintStop(const json::Dict& request_map) const {
	json::Node result;
	const std::string& stop_name = request_map.at("name").AsString();
	const int id = request_map.at("id").AsInt();
	if (!db_.ContainsStop(stop_name)) {
		result = json::Builder{}
				.StartDict()
					.Key("error_message").Value("not found")
					.Key("request_id").Value(id)
				.EndDict()
			.Build();
	}
	else {
		json::Array buses;
		for (auto& bus : db_.GetBusesByStop(stop_name)) {
			buses.push_back(std::string(bus));
		}
		result = json::Builder{}
				.StartDict()
					.Key("buses").Value(buses)
					.Key("request_id").Value(id)
				.EndDict()
			.Build();
	}
	return result;
}

json::Node RequestHandler::PrintMap(const json::Dict& request_map, render::MapRender& map_render) const {
	json::Node result;
	const int id = request_map.at("id").AsInt();
	std::stringstream output;
	map_render.CreateMap(db_.GetAllBusesSorted()).Render(output);
	result = json::Builder{}
			.StartDict()
				.Key("map").Value(output.str())
				.Key("request_id").Value(id)
			.EndDict()
		.Build();
	return result;
}

std::optional<catalogue::BusStat> RequestHandler::GetBusStat(const std::string_view bus_number) const {
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