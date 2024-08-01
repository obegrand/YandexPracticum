#include "json_reader.h"
#include <map>
#include "transport_router.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

const json::Node& JsonReader::GetBaseRequests() const {
	if (!json_doc_.GetRoot().AsMap().count("base_requests")) throw std::out_of_range("Not contain");
	return json_doc_.GetRoot().AsMap().at("base_requests");
}

const json::Node& JsonReader::GetStatRequests() const {
	if (!json_doc_.GetRoot().AsMap().count("stat_requests")) throw std::out_of_range("Not contain");
	return json_doc_.GetRoot().AsMap().at("stat_requests");
}

const json::Node& JsonReader::GetRenderSettings() const {
	if (!json_doc_.GetRoot().AsMap().count("render_settings")) throw std::out_of_range("Not contain");
	return json_doc_.GetRoot().AsMap().at("render_settings");
}

const json::Node& JsonReader::GetRoutingSettings() const {
	if (!json_doc_.GetRoot().AsMap().count("routing_settings")) throw std::out_of_range("Not contain");
	return json_doc_.GetRoot().AsMap().at("routing_settings");
}

void JsonReader::FillCatalogue() {
	if (GetBaseRequests() == nullptr) return;
	const json::Array& base_requests = GetBaseRequests().AsArray();
	for (auto& request : base_requests) {
		if (request.AsMap().at("type").AsString() == "Stop") {
			AddStop(request.AsMap());
		}
	}
	for (auto& request : base_requests) {
		if (request.AsMap().at("type").AsString() == "Stop") {
			SetDistances(request.AsMap());
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
	catalogue_.AddStop(stop_name, coordinates);
}

void JsonReader::SetDistances(const json::Dict& data) {
	for (auto& [other_stop_name, dist] : data.at("road_distances").AsMap()) {
		catalogue_.SetDistanceBetweenStops(
			&catalogue_.GetStop(data.at("name").AsString()),
			&catalogue_.GetStop(other_stop_name),
			dist.AsInt());
	}
}

void JsonReader::AddBus(const json::Dict& data) {
	std::vector<std::string_view> stop_names;
	bool is_roundtrip = data.at("is_roundtrip").AsBool();
	for (auto& stop_name : data.at("stops").AsArray()) {
		stop_names.push_back(stop_name.AsString());
	}
	//if (!is_roundtrip && stop_names.front() == stop_names.back()) is_roundtrip = !is_roundtrip;
	if (!is_roundtrip) {
		for (int i = stop_names.size() - 2; i >= 0; --i) {
			stop_names.push_back(stop_names[i]);
		}
	}
	catalogue_.AddBus(data.at("name").AsString(), stop_names, is_roundtrip);
}

static svg::Color NodeAsColor(json::Node node) {
	if (node.IsArray()) {
		if (node.AsArray().size() == 4) {
			svg::Rgba rgba;
			rgba.red = node.AsArray()[0].AsInt();
			rgba.green = node.AsArray()[1].AsInt();
			rgba.blue = node.AsArray()[2].AsInt();
			rgba.opacity = node.AsArray()[3].AsDouble();
			return rgba;
		}
		else {
			svg::Rgb rgb;
			rgb.red = node.AsArray()[0].AsInt();
			rgb.green = node.AsArray()[1].AsInt();
			rgb.blue = node.AsArray()[2].AsInt();
			return rgb;
		}
	}
	else {
		return node.AsString();
	}
}

render::RenderSettings JsonReader::FillRenderSettings(const json::Dict& settings_map) const {
	render::RenderSettings render_settings;
	render_settings.width = settings_map.at("width").AsDouble();
	render_settings.height = settings_map.at("height").AsDouble();
	render_settings.padding = settings_map.at("padding").AsDouble();
	render_settings.stop_radius = settings_map.at("stop_radius").AsDouble();
	render_settings.line_width = settings_map.at("line_width").AsDouble();
	render_settings.bus_label_font_size = settings_map.at("bus_label_font_size").AsInt();
	const json::Array& bus_label_offset = settings_map.at("bus_label_offset").AsArray();
	render_settings.bus_label_offset = { bus_label_offset[0].AsDouble(), bus_label_offset[1].AsDouble() };
	render_settings.stop_label_font_size = settings_map.at("stop_label_font_size").AsInt();
	const json::Array& stop_label_offset = settings_map.at("stop_label_offset").AsArray();
	render_settings.stop_label_offset = { stop_label_offset[0].AsDouble(), stop_label_offset[1].AsDouble() };

	render_settings.underlayer_color = NodeAsColor(settings_map.at("underlayer_color"));

	render_settings.underlayer_width = settings_map.at("underlayer_width").AsDouble();

	for (auto& node : settings_map.at("color_palette").AsArray()) {
		render_settings.color_palette.emplace_back(NodeAsColor(node));
	}

	return render_settings;
}

transport::RoutingSettings JsonReader::FillRoutingSettings(const json::Dict& settings_map) const {
	transport::RoutingSettings routing_settings;
	routing_settings.bus_wait_time_ = settings_map.at("bus_wait_time").AsInt();
	routing_settings.bus_velocity_ = settings_map.at("bus_velocity").AsDouble();
	return routing_settings;
}

