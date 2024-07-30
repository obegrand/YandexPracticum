#pragma once
#include <string>
#include "geo.h"
#include <unordered_map>
#include <vector>

/*
 * В этом файле вы можете разместить классы/структуры, которые являются частью предметной области (domain)
 * вашего приложения и не зависят от транспортного справочника. Например Автобусные маршруты и Остановки.
 *
 * Их можно было бы разместить и в transport_catalogue.h, однако вынесение их в отдельный
 * заголовочный файл может оказаться полезным, когда дело дойдёт до визуализации карты маршрутов:
 * визуализатор карты (map_renderer) можно будет сделать независящим от транспортного справочника.
 *
 * Если структура вашего приложения не позволяет так сделать, просто оставьте этот файл пустым.
 *
 */


namespace catalogue {

	struct Stop {
		std::string name;
		geo::Coordinates coordinates;
		std::unordered_map<std::string_view, double> distances_to_other_stops;
	};

	struct Bus {
		std::string number;
		std::vector<Stop*> stop_names;
		bool is_roundtrip;
	};

	struct BusStat {
		int stops_count;
		int unique_stops_count;
		double route_length;
		double curvature;
	};

} // namespace TransportCatalogue