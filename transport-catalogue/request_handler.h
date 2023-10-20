#pragma once

#include "transport_catalogue.h"
#include "map_renderer.h"
#include <sstream>
#include <fstream>
#include <iomanip>

namespace request_handler {
    class RequestHandler : public catalogue::TransportCatalogue{
    public:
        RequestHandler(TransportCatalogue& catalog);

        std::optional<BusInfo> GetBusStat(const std::string_view& bus_name) const;

        StopInfo GetBusesByStop(const std::string_view& stop_name) const;

    private:
        TransportCatalogue& catalog_;
    };
}