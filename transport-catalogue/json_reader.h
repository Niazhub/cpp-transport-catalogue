#pragma once

#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json.h"
#include "json_builder.h"
#include "transport_router.h"
#include "serialization.h"
#include <sstream>
#include <fstream>
#include <string>

namespace json_reader {
    class JsonReader : public request_handler::RequestHandler, public renderer::MapRenderer {
    public:
        JsonReader(TransportCatalogue& catalog);

        void MakeBase();
        void ProcessRequests();
        void OutData();

    private:
        TransportCatalogue& catalog_;
        Domain domain;
        string serialization_file;
        void ParserStops();
        void ParserBuses();
        void ParserDistance();
        void ParserMakeBaseQueryes(const json::Dict& dict);
        void ParserProcessRequestsQueryes(const json::Dict& dict);
        void ParserBaseRequests(const std::pair<std::string, json::Node>& value_1);
        void ParserStatRequests(const std::pair<std::string, json::Node>& value_1);
        void ParserRenderSettings(const std::pair<std::string, json::Node>& value_1);
        void ParserRoutingSettings(const std::pair<std::string, json::Node>& value_1);
        void ParserSerializationSettings(const std::pair<std::string, json::Node>& value_1);
    };
}