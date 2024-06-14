#include "json_reader.h"
#include <map>

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

const json::Node& JsonReader::GetBaseRequests() const {
	if (!json_doc_.GetRoot().AsMap().count("base_requests")) return null;
	return json_doc_.GetRoot().AsMap().at("base_requests");
}

const json::Node& JsonReader::GetStatRequests() const {
	if (!json_doc_.GetRoot().AsMap().count("stat_requests")) return null;
	return json_doc_.GetRoot().AsMap().at("stat_requests");
}

void JsonReader::FillCatalogue() {
	const json::Array& base_requests = GetBaseRequests().AsArray();
	for (auto& request : base_requests) {
		if (request.AsMap().at("type").AsString() == "Stop") {
			AddStop(request.AsMap());
		}
	}
	for (auto& request : base_requests) {
		if (request.AsMap().at("type").AsString() == "Bus") {
			AddBus(request.AsMap());
		}
	}
}

void JsonReader::AddStop(const json::Dict& data) {
	std::string stop_name = data.at("name").AsString();
	geo::Coordinates coordinates = { data.at("latitude").AsDouble(), data.at("longitude").AsDouble() };
	std::unordered_map<std::string, double> stop_distances;
	auto& distances = data.at("road_distances").AsMap();
	for (auto& [stop_name, dist] : distances) {
		stop_distances.emplace(stop_name, dist.AsInt());
	}
	catalogue_.Add(stop_name, coordinates, stop_distances);
}

void JsonReader::AddBus(const json::Dict& data) {
	std::string stop_name = data.at("name").AsString();
	std::vector<std::string_view> stop_names;
	for (auto& stop_name : data.at("stops").AsArray()) {
		stop_names.push_back(stop_name.AsString());
	}
	if (!data.at("is_roundtrip").AsBool()) {
		for (int i = stop_names.size() - 2; i >= 0; --i) {
			stop_names.push_back(stop_names[i]);
		}
	}
	catalogue_.Add(stop_name, stop_names);
}