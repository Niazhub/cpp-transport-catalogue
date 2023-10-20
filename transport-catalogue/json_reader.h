#pragma once

#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json.h"
#include "json_builder.h"
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
        void ParserStops();
        void ParserBuses();
        void ParserDistance();
        void ParserQueryes(const json::Dict& dict);
        void ParserBaseRequests(const std::pair<std::string, json::Node>& value_1);
        void ParserStatRequests(const std::pair<std::string, json::Node>& value_1);
        void ParserRenderSettings(const std::pair<std::string, json::Node>& value_1);
    };
}