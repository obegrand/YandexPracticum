#pragma once
#include "json.h"
#include "transport_catalogue.h"
#include "transport_router.h"
#include "map_renderer.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

class JsonReader {
public:
	JsonReader(std::istream& input, catalogue::TransportCatalogue& catalogue)
		: catalogue_(catalogue), json_doc_(json::Load(input)) { }

	void FillCatalogue();

	const json::Node& GetBaseRequests() const;
	const json::Node& GetStatRequests() const;
	const json::Node& GetRenderSettings() const;
	const json::Node& GetRoutingSettings() const;

	void AddStop(const json::Dict& stop);
	void SetDistances(const json::Dict& data);
	void AddBus(const json::Dict& bus);

	render::RenderSettings FillRenderSettings(const json::Dict& settings_map) const;

	transport::RoutingSettings FillRoutingSettings(const json::Dict& settings_map) const;


private:
	catalogue::TransportCatalogue& catalogue_;
	const json::Document json_doc_;
};