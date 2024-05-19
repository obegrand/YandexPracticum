#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace Statistic {

void ParseAndPrintStat(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
    std::string_view request, std::ostream& output);

void PrintBus(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
    std::string_view request, std::ostream& output);

void PrintStop(const TransportCatalogue::TransportCatalogue& tansport_catalogue,
    std::string_view request, std::ostream& output);

} // namespace Statistic