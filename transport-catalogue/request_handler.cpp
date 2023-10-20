#include "request_handler.h"

request_handler::RequestHandler::RequestHandler(TransportCatalogue& catalog)
    : catalog_(catalog) {}

std::optional<domain::Domain::BusInfo> request_handler::RequestHandler::GetBusStat(const std::string_view& bus_name) const {
    BusInfo info = catalog_.GetBusInfo(bus_name);

    if (info.total_stops == 0 || info.unique_stops == 0 || info.route_length == 0.0 || info.curvature == 0.0) {
        return {};
    }
    else {
        return { { info.total_stops, info.unique_stops, info.route_length, info.curvature } };
    }
}

domain::Domain::StopInfo request_handler::RequestHandler::GetBusesByStop(const std::string_view& stop_name) const {
    domain::Domain::StopInfo info = catalog_.GetStopInfo(stop_name);
    std::set<std::string> buses;
    if (info.is_some_bus == false) {
        return { info.buses, info.is_some_bus };
    }
    else {
        for (const auto& value : info.buses) {
            if (info.is_some_bus == false) {
                break;
            }
            buses.insert(value);
        }
        return { buses, info.is_some_bus };
    }
}