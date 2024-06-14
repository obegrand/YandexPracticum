#pragma once
#include "json.h"
#include "transport_catalogue.h"

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

	void AddStop(const json::Dict& stop);
	void AddBus(const json::Dict& bus);

private:
	catalogue::TransportCatalogue& catalogue_;
	const json::Document json_doc_;
	json::Node null = nullptr;
};