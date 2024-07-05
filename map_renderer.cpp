#include "map_renderer.h"

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршртутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */

namespace render {

	bool IsZero(double value) {
		return std::abs(value) < EPSILON;
	}

	svg::Point SphereProjector::operator()(geo::Coordinates coords) const {
		return {
			(coords.lng - min_lon_) * zoom_coeff_ + padding_,
			(max_lat_ - coords.lat) * zoom_coeff_ + padding_
		};
	}


	std::vector<svg::Polyline> MapRender::CalculateRoutes(const std::map<std::string_view, const catalogue::Bus*>& buses, const SphereProjector& sphere_projector) const {
		std::vector<svg::Polyline> result;
		size_t color_num = 0;
		for (const auto& [bus_number, bus] : buses) {
			if (bus->stop_names.empty()) continue;
			std::vector<const catalogue::Stop*> route_stops{ bus->stop_names.begin(), bus->stop_names.end() };
			svg::Polyline line;
			for (const auto& stop : route_stops) {
				line.AddPoint(sphere_projector(stop->coordinates));
			}
			line.SetStrokeColor(settings_.color_palette[color_num]);
			line.SetFillColor("none");
			line.SetStrokeWidth(settings_.line_width);
			line.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
			line.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

			if (color_num < (settings_.color_palette.size() - 1)) ++color_num;
			else color_num = 0;

			result.push_back(line);
		}

		return result;
	}

	std::vector<svg::Text> MapRender::CalculateRoutesNames(const std::map<std::string_view, const catalogue::Bus*>& buses, const SphereProjector& sphere_projector) const {
		std::vector<svg::Text> result;
		size_t color_num = 0;
		for (const auto& [bus_number, bus] : buses) {
			if (bus->stop_names.empty()) continue;
			// Общие свойства обоих объектов
			svg::Text route_name_default;
			route_name_default.SetPosition(sphere_projector(bus->stop_names.at(0)->coordinates));
			route_name_default.SetOffset(settings_.bus_label_offset);
			route_name_default.SetFontSize(settings_.bus_label_font_size);
			route_name_default.SetFontFamily("Verdana");
			route_name_default.SetFontWeight("bold");
			route_name_default.SetData(bus->number);
			// Дополнительные свойства подложки
			svg::Text route_name_backlayer = route_name_default;
			route_name_backlayer.SetFillColor(settings_.underlayer_color);
			route_name_backlayer.SetStrokeColor(settings_.underlayer_color);
			route_name_backlayer.SetStrokeWidth(settings_.underlayer_width);
			route_name_backlayer.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
			route_name_backlayer.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
			result.push_back(route_name_backlayer);
			// Дополнительное свойство самой надписи
			svg::Text route_name = route_name_default;
			route_name.SetFillColor(settings_.color_palette[color_num]);
			result.push_back(route_name);
			//Если некольцевая добавляет надпись второй конечной станции
			if (!bus->is_roundtrip && bus->stop_names.at(0) != bus->stop_names.at(bus->stop_names.size() / 2)) {
				route_name_backlayer.SetPosition(sphere_projector(bus->stop_names.at(bus->stop_names.size()/2)->coordinates));
				route_name.SetPosition(sphere_projector(bus->stop_names.at(bus->stop_names.size()/2)->coordinates));
				result.push_back(route_name_backlayer);
				result.push_back(route_name);
			}

			if (color_num < (settings_.color_palette.size() - 1)) ++color_num;
			else color_num = 0;
		}
		return result;
	}

	static std::map<std::string_view, catalogue::Stop*> ComputeStops(const std::map<std::string_view, const catalogue::Bus*>& buses) {
		std::map<std::string_view, catalogue::Stop*> stops;
		for (const auto& [bus_number, bus] : buses) {
			if (bus->stop_names.empty()) continue;
			for (const auto& stop : bus->stop_names) {
				stops[stop->name] = stop;
			}
		}
		return stops;
	}

	std::vector<svg::Circle> MapRender::CalculateStopSigns(const std::map<std::string_view, catalogue::Stop*>& stops, const SphereProjector& sphere_projector) const {
		std::vector<svg::Circle> result;
		for (const auto& [stop_name, stop] : stops) {
			svg::Circle stop_sign;
			stop_sign.SetCenter(sphere_projector(stop->coordinates));
			stop_sign.SetRadius(settings_.stop_radius);
			stop_sign.SetFillColor("white");
			result.push_back(stop_sign);
		}
		return result;
	}

	std::vector<svg::Text> MapRender::CalculateStopNames(const std::map<std::string_view, catalogue::Stop*>& stops, const SphereProjector& sphere_projector) const {
		std::vector<svg::Text> result;
		for (const auto& [stop_name, stop] : stops) {
			// Общие свойства обоих объектов
			svg::Text stop_name_default;
			stop_name_default.SetPosition(sphere_projector(stop->coordinates));
			stop_name_default.SetOffset(settings_.stop_label_offset);
			stop_name_default.SetFontSize(settings_.stop_label_font_size);
			stop_name_default.SetFontFamily("Verdana");
			stop_name_default.SetData(stop->name);
			// Дополнительные свойства подложки
			svg::Text stop_name_backlayer = stop_name_default;
			stop_name_backlayer.SetFillColor(settings_.underlayer_color);
			stop_name_backlayer.SetStrokeColor(settings_.underlayer_color);
			stop_name_backlayer.SetStrokeWidth(settings_.underlayer_width);
			stop_name_backlayer.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
			stop_name_backlayer.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
			result.push_back(stop_name_backlayer);
			// Дополнительное свойство самой надписи
			svg::Text stop_name_front = stop_name_default;
			stop_name_front.SetFillColor("black");
			result.push_back(stop_name_front);
		}
		return result;
	}

	svg::Document MapRender::CreateMap(const std::map<std::string_view, const catalogue::Bus*>& buses) const {
		svg::Document result;
		std::vector<geo::Coordinates> route_stops_coord;

		for (const auto& [bus_number, bus] : buses) {
			for (const auto& stop : bus->stop_names) {
				route_stops_coord.push_back(stop->coordinates);
			}
		}

		SphereProjector sphere_projector(route_stops_coord.begin(), route_stops_coord.end(), settings_.width, settings_.height, settings_.padding);

		for (const auto& line : CalculateRoutes(buses, sphere_projector)) {
			result.Add(line);
		}

		for (const auto& route_name : CalculateRoutesNames(buses, sphere_projector)) {
			result.Add(route_name);
		}

		std::map<std::string_view, catalogue::Stop*> stops = ComputeStops(buses);
		for (const auto& stop_sign : CalculateStopSigns(stops, sphere_projector)) {
			result.Add(stop_sign);
		}
		for (const auto& stop_name : CalculateStopNames(stops, sphere_projector)) {
			result.Add(stop_name);
		}

		return result;
	}

} // namespace render
