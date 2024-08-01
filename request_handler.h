/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */
#pragma once
#include <optional>
#include <unordered_set>
#include "transport_catalogue.h"
#include "json_reader.h"
#include "json.h"
#include "transport_router.h"

class RequestHandler {
public:
	RequestHandler(JsonReader& requests, catalogue::TransportCatalogue& catalogue, transport::Router& router);
	void ProcessRequests() const;

	const json::Node PrintBus(const json::Dict& request_map) const;

	const json::Node PrintStop(const json::Dict& request_map) const;

	const json::Node PrintRoute(const json::Dict& request_map) const;

	json::Node PrintMap(const json::Dict& request_map, render::MapRender& map_render) const;

	std::optional<catalogue::BusStat> GetBusStat(const std::string_view bus_number) const;

private:
	const JsonReader& requests_;
	const catalogue::TransportCatalogue& db_;
	const transport::Router& router_;
};
