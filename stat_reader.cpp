#include "stat_reader.h"

void ParseAndPrintStat(const TransportCatalogue& tansport_catalogue,
	std::string_view request, std::ostream& output) {
	output << request << ": ";
	request.remove_prefix(request.find(' ') + 1);

	if (!tansport_catalogue.ContainsBus(request)) {
		output << "not found" << std::endl;
	}
	else
	{
		Bus bus = tansport_catalogue.GetBus(request);
		output << bus.stops.size() << " stops on route, ";
		std::set<std::string> countUniqueStops;
		for (const auto& stop : bus.stops) {
			countUniqueStops.insert(stop);
		}
		output << countUniqueStops.size()<<" unique stops, " << tansport_catalogue.ComputeBusDistance(bus) << " route length" << std::endl;
	}
}