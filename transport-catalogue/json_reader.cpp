#include "json_reader.h"

json_reader::JsonReader::JsonReader(TransportCatalogue& catalog)
    : RequestHandler(catalog), catalog_(catalog) {}

void json_reader::JsonReader::ParserStops() {
    for (const auto& value : catalogue::TransportCatalogue::struct_stop_base) {
        if (value.type == "Stop") {
            catalog_.AddStop({ value.name, {value.latitude, value.longitude} });
        }
    }
}

void json_reader::JsonReader::ParserBuses() {
    for (const auto& value : catalogue::TransportCatalogue::struct_bus_base) {
        if (value.type == "Bus") {
            std::vector<TransportCatalogue::Stop*> stops;

            if (value.is_roundtrip) {
                for (const auto& val : value.stops) {
                    if (catalog_.FindStop(val) != nullptr) {
                        stops.push_back(catalog_.FindStop(val));
                    }
                }
            }
            else {
                auto res = value.stops;
                for (const auto& vec : res) {
                    if (catalog_.FindStop(vec) != nullptr) {
                        stops.push_back(catalog_.FindStop(vec));
                    }
                }
                res.pop_back();
                std::reverse(res.begin(), res.end());
                for (const auto& vec : res) {
                    if (catalog_.FindStop(vec) != nullptr) {
                        stops.push_back(catalog_.FindStop(vec));
                    }
                }
            }
            renderer::MapRenderer::SetBuses({ value.name, stops, value.is_roundtrip });
            catalog_.AddBus({ value.name, stops, value.is_roundtrip });
        }
    }
}

void json_reader::JsonReader::ParserDistance() {
    for (const auto& value : catalogue::TransportCatalogue::struct_stop_base) {
        if (value.type == "Stop") {
            for (const auto& val : value.road_distances) {
                catalog_.AddStopDistance(value.name, val.first, val.second);
            }
        }
    }
}

void json_reader::JsonReader::ParserBaseRequests(const std::pair<std::string, json::Node>& value_1) {
    std::string name;
    BusQueryes bus;
    StopQueryes stop;
    for (const auto& value_2 : value_1.second.AsArray()) {
        std::string type;
        for (const auto& value_3 : value_2.AsDict()) {
            if (value_3.first == "type") {
                if (value_3.second.AsString() == "Bus") {
                    bus.type = value_3.second.AsString();
                    if (name != "") {
                        bus.name = name;
                        name = "";
                    }
                    type = value_3.second.AsString();
                }
                else if (value_3.second.AsString() == "Stop") {
                    stop.type = value_3.second.AsString();
                    if (name != "") {
                        stop.name = name;
                        name = "";
                    }
                    type = value_3.second.AsString();
                }
            }
            else if (value_3.first == "name") {
                if (type == "Bus") {
                    bus.name = value_3.second.AsString();
                }
                else if (type == "Stop") {
                    stop.name = value_3.second.AsString();
                }
                else {
                    name = value_3.second.AsString();
                }
            }
            else if (value_3.first == "stops") {
                std::vector<std::string> vec;
                for (const auto& value_4 : value_3.second.AsArray()) {
                    vec.push_back(value_4.AsString());
                }
                bus.stops = vec;
            }
            else if (value_3.first == "is_roundtrip") {
                bus.is_roundtrip = value_3.second.AsBool();
            }
            else if (value_3.first == "road_distances") {
                std::unordered_map<std::string, double> map;
                for (const auto& value_4 : value_3.second.AsDict()) {
                    map.insert({ value_4.first , value_4.second.AsDouble() });
                }
                stop.road_distances = map;
            }
            else if (value_3.first == "longitude") {
                stop.longitude = value_3.second.AsDouble();
            }
            else if (value_3.first == "latitude") {
                stop.latitude = value_3.second.AsDouble();
            }
        }
        if (type == "Bus") {
            catalogue::TransportCatalogue::struct_bus_base.push_back(bus);
        }
        else if (type == "Stop") {
            catalogue::TransportCatalogue::struct_stop_base.push_back(stop);
        }
    }
}

void json_reader::JsonReader::ParserStatRequests(const std::pair<std::string, json::Node>& value_1) {
    DataStat stat_data;
    for (const auto& value_2 : value_1.second.AsArray()) {
        std::string type_query;
        for (const auto& value_3 : value_2.AsDict()) {
            if (value_3.first == "type") {
                if (value_3.second.AsString() == "Bus") {
                    stat_data.type = value_3.second.AsString();
                    type_query = "Bus";
                }
                else if (value_3.second.AsString() == "Stop") {
                    stat_data.type = value_3.second.AsString();
                    type_query = "Stop";
                }
                else if (value_3.second.AsString() == "Map") {
                    stat_data.type = value_3.second.AsString();
                    type_query = "Map";
                }
            }
            else if (value_3.first == "name") {
                stat_data.name = value_3.second.AsString();
            }
            else if (value_3.first == "id") {
                stat_data.id = value_3.second.AsInt();
            }
        }
        if (type_query == "Bus") {
            catalogue::TransportCatalogue::struct_data_stat.push_back(stat_data);
        }
        else if (type_query == "Stop") {
            catalogue::TransportCatalogue::struct_data_stat.push_back(stat_data);
        }
        else if (type_query == "Map") {
            catalogue::TransportCatalogue::struct_data_stat.push_back(stat_data);
        }
    }
}

void json_reader::JsonReader::ParserRenderSettings(const std::pair<std::string, json::Node>& value_1) {
    for (const auto& value : value_1.second.AsDict()) {
        if (value.first == "width") {
            domain::Domain::rend_color.width = value.second.AsDouble();
        }
        else if (value.first == "height") {
            domain::Domain::rend_color.height = value.second.AsDouble();
        }
        else if (value.first == "padding") {
            domain::Domain::rend_color.padding = value.second.AsDouble();
        }
        else if (value.first == "line_width") {
            domain::Domain::rend_color.line_width = value.second.AsDouble();
        }
        else if (value.first == "stop_radius") {
            domain::Domain::rend_color.stop_radius = value.second.AsDouble();
        }
        else if (value.first == "bus_label_font_size") {
            domain::Domain::rend_color.bus_label_font_size = value.second.AsInt();
        }
        else if (value.first == "bus_label_offset") {
            std::vector<double> vec;
            for (const auto& value_1 : value.second.AsArray()) {
                vec.push_back(value_1.AsDouble());
            }
            domain::Domain::rend_color.bus_label_offset = vec;
        }
        else if (value.first == "stop_label_font_size") {
            domain::Domain::rend_color.stop_label_font_size = value.second.AsInt();
        }
        else if (value.first == "stop_label_offset") {
            std::vector<double> vec;
            for (const auto& value_1 : value.second.AsArray()) {
                vec.push_back(value_1.AsDouble());
            }
            domain::Domain::rend_color.stop_label_offset = vec;
        }
        else if (value.first == "underlayer_color") {
            if (value.second.IsString()) {
                domain::Domain::rend_color.underlayer_color = value.second.AsString();
            }
            else if (value.second.IsArray()) {
                if (value.second.AsArray().size() == 3) {
                    domain::Domain::rend_color.underlayer_color = svg::Rgb(value.second.AsArray()[0].AsInt(), value.second.AsArray()[1].AsInt(), value.second.AsArray()[2].AsInt());
                }
                else if (value.second.AsArray().size() == 4) {
                    domain::Domain::rend_color.underlayer_color = svg::Rgba(value.second.AsArray()[0].AsInt(), value.second.AsArray()[1].AsInt(), value.second.AsArray()[2].AsInt(), value.second.AsArray()[3].AsDouble());
                }
            }
        }
        else if (value.first == "underlayer_width") {
            domain::Domain::rend_color.underlayer_width = value.second.AsDouble();
        }
        else if (value.first == "color_palette") {
            for (const auto& value_1 : value.second.AsArray()) {
                if (value_1.IsString()) {
                    domain::Domain::rend_color.color_palette.push_back(value_1.AsString());
                }
                else if (value_1.IsArray()) {
                    if (value_1.AsArray().size() == 3) {
                        domain::Domain::rend_color.color_palette.push_back(svg::Rgb(value_1.AsArray()[0].AsInt(), value_1.AsArray()[1].AsInt(), value_1.AsArray()[2].AsInt()));
                    }
                    else if (value_1.AsArray().size() == 4) {
                        domain::Domain::rend_color.color_palette.push_back(svg::Rgba(value_1.AsArray()[0].AsInt(), value_1.AsArray()[1].AsInt(), value_1.AsArray()[2].AsInt(), value_1.AsArray()[3].AsDouble()));
                    }
                }
            }
        }
    }
}

void json_reader::JsonReader::ParserQueryes(const json::Dict& dict) {
    std::string query;

    for(const auto& value_1 : dict) {
        if (value_1.first == "base_requests") {
            query = value_1.first;
        }
        else if(value_1.first == "stat_requests") {
            query = value_1.first;
        }
        else if (value_1.first == "render_settings") {
            query = value_1.first;
        }
        if (query == "base_requests") {
            ParserBaseRequests(value_1);
        }
        else if (query == "stat_requests") {
            ParserStatRequests(value_1);
        }
        else if (query == "render_settings") {
            ParserRenderSettings(value_1);
        }
    }
}

void json_reader::JsonReader::InputData() {
    std::string input;
    std::ostringstream input_for_parse;
    int braceCount = 0;
    while (std::getline(std::cin, input)) {
        input_for_parse << input;

        for (char c : input) {
            if (c == '{') {
                braceCount++;
            }
            else if (c == '}') {
                braceCount--;
            }

            if (braceCount == 0) {
                break;
            }
        }

        if (braceCount == 0) {
            break;
        }
    }

    std::string strochka = input_for_parse.str();
    std::istringstream s(strochka);
    std::stringstream oss;
    json::Dict dict = json::Load(s).GetRoot().AsDict();
    ParserQueryes(dict);

    ParserStops();
    ParserBuses();
    ParserDistance();

    json::Array array;

    for (const auto& value : domain::Domain::struct_data_stat) {
        if (value.type == "Bus") {
            std::optional<BusInfo> bus_result = GetBusStat(value.name);
            if (bus_result.has_value()) {
                array.emplace_back(json::Builder{}
                    .StartDict()
                        .Key("curvature").Value(bus_result.value().curvature)
                        .Key("request_id").Value(value.id)
                        .Key("route_length").Value(bus_result.value().route_length)
                        .Key("stop_count").Value(bus_result.value().total_stops)
                        .Key("unique_stop_count").Value(bus_result.value().unique_stops)
                    .EndDict().Build());
            }
            else {
                std::string str = "not found";
                array.emplace_back(json::Builder{}
                    .StartDict()
                        .Key("request_id").Value(value.id)
                        .Key("error_message").Value(str)
                    .EndDict().Build());
            }
        }
        else if (value.type == "Stop") {
            StopInfo stop_result = GetBusesByStop(value.name);
            if (stop_result.is_some_bus == false) {
                std::string str = "not found";
                array.emplace_back(json::Builder{}
                    .StartDict()
                        .Key("request_id").Value(value.id)
                        .Key("error_message").Value(str)
                    .EndDict().Build());
            }
            else if (stop_result.is_some_bus == true) {
                json::Array arr;
                for (const auto& val : stop_result.buses) {
                    arr.emplace_back(val);
                }
                array.emplace_back(json::Builder{}
                    .StartDict()
                        .Key("buses").Value(arr)
                        .Key("request_id").Value(value.id)
                    .EndDict().Build());
            }
        }
        else if (value.type == "Map") {
            SetColorSettings(domain::Domain::rend_color);
            std::ostringstream out;
            RenderBuses(out);
            std::string str = out.str();
            array.emplace_back(json::Builder{}
                .StartDict()
                    .Key("map").Value(str)
                    .Key("request_id").Value(value.id)
                .EndDict().Build());
        }
    }
    json::Print(json::Document{ array }, std::cout);
}