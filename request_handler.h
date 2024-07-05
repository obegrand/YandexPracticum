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

 //class RequestHandler {
 //public:
 //    // MapRenderer понадобится в следующей части итогового проекта
 //    // RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);
 //
 //    // Возвращает информацию о маршруте (запрос Bus)
 //    std::optional<catalogue::BusStat> GetBusStat(const std::string_view& bus_name) const;
 //
 //    // Возвращает маршруты, проходящие через
 //    const std::unordered_set<catalogue::Bus>* GetBusesByStop(const std::string_view& stop_name) const;
 //
 //    // Этот метод будет нужен в следующей части итогового проекта
 //    // svg::Document RenderMap() const;
 //
 //private:
 //    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
 //    const catalogue::TransportCatalogue& db_;
 //    // const renderer::MapRenderer& renderer_;
 //};

#pragma once
#include <optional>
#include <unordered_set>
#include "transport_catalogue.h"
#include "json_reader.h"
#include "json.h"

class RequestHandler {
public:
	RequestHandler(JsonReader& requests, catalogue::TransportCatalogue& catalogue);
	void ProcessRequests() const;

	const json::Node PrintBus(const json::Dict& request_map) const;
	
	const json::Node PrintStop(const json::Dict& request_map) const;

	json::Node PrintMap(const json::Dict& request_map, render::MapRender& map_render) const;

	std::optional<catalogue::BusStat> GetBusStat(const std::string_view bus_number) const;

	const std::set<std::string_view> GetBusesByStop(std::string_view stop_name) const;

private:
	const JsonReader& requests_;
	const catalogue::TransportCatalogue& db_;
};
