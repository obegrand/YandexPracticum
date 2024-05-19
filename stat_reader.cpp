#include "stat_reader.h"

namespace Statistic {

void ParseAndPrintStat(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
	std::string_view request, std::ostream& output) {
	std::string_view command = request.substr(0, request.find(' '));
	output << request << ": ";
	request.remove_prefix(request.find(' ') + 1);

	if (command == "Bus") {
		PrintBus(tansport_catalogue, request, output);
	}
	else if (command == "Stop") {
		PrintStop(tansport_catalogue, request, output);
	}
}

void PrintBus(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
	std::string_view request, std::ostream& output) {
	if (!tansport_catalogue.ContainsBus(request)) {
		output << "not found" << std::endl;
	}
	else {
		TransportCatalogue::Bus bus = tansport_catalogue.GetBus(request);
		output << bus.stops.size() << " stops on route, ";
		std::set<std::string> countUniqueStops;
		for (const auto& stop : bus.stops) {
			countUniqueStops.insert(stop);
		}
		output << countUniqueStops.size() << " unique stops, "
			<< tansport_catalogue.ComputeBusDistance(bus)
			<< " route length" << std::endl;
	}
}

void PrintStop(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
	std::string_view request, std::ostream& output) {
	if (!tansport_catalogue.ContainsStop(request)) {
		output << "not found" << std::endl;
	}
	else {
		std::set<std::string> buses = tansport_catalogue.GetBusesByStop(request);
		if (buses.empty()) {
			output << "no buses" << std::endl;
		}
		else {
			output << "buses";
			for (const auto& bus : buses) {
				output << " " << bus;
			}
			output << std::endl;
		}
	}
}

} // namespace Statistic