#pragma once

#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json.h"
#include "json_builder.h"
#include "transport_router.h"
#include "log_duration.h"
#include <sstream>
#include <fstream>
#include <string>

namespace json_reader {
    class JsonReader : public request_handler::RequestHandler, public renderer::MapRenderer{
    public:
        JsonReader(TransportCatalogue& catalog);

        void InputData();

    private:
        TransportCatalogue& catalog_;
        void ParserStops(domain::Domain& domain);
        void ParserBuses(domain::Domain& domain);
        void ParserDistance(domain::Domain& domain);
        void ParserQueryes(const json::Dict& dict, domain::Domain& domain);
        void ParserBaseRequests(const std::pair<std::string, json::Node>& value_1, domain::Domain& domain);
        void ParserStatRequests(const std::pair<std::string, json::Node>& value_1, domain::Domain& domain);
        void ParserRenderSettings(const std::pair<std::string, json::Node>& value_1, domain::Domain& domain);
        void ParserRoutingSettings(const std::pair<std::string, json::Node>& value_1, domain::Domain& domain);
    };
}