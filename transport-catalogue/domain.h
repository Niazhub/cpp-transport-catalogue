#pragma once
#include "geo.h"
#include "svg.h"
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

namespace domain {
	class Domain : public geo::Coordinates {
    public:
        struct Stop {
            std::string stop_name_;
            geo::Coordinates stop_coordinates_;
        };

        struct Bus {
            std::string bus_name_;
            std::vector<Stop*> bus_to_stops_;
            bool is_roundtrip;
        };

        struct BusInfo {
            int total_stops = 0;
            int unique_stops = 0;
            double route_length = 0.0;
            double curvature = 0.0;
        };

        struct StopInfo {
            std::set<std::string> buses;
            bool is_some_bus;
        };

        struct BusQueryes {
            std::string type;
            std::string name;
            std::vector<std::string> stops;
            bool is_roundtrip;
        };

        struct StopQueryes {
            std::string type;
            std::string name;
            double latitude;
            double longitude;
            std::unordered_map<std::string, double> road_distances;
        };

        struct DataStat {
            int id;
            std::string type;
            std::string name;
        };

        struct RendererColor {
            double width;
            double height;
            double padding;
            double line_width;
            double stop_radius;
            int bus_label_font_size;
            std::vector<double> bus_label_offset;
            int stop_label_font_size;
            std::vector<double> stop_label_offset;
            svg::Color underlayer_color;
            double underlayer_width;
            std::vector<svg::Color> color_palette;
        };

        std::vector<BusQueryes> struct_bus_base;
        std::vector<StopQueryes> struct_stop_base;
        std::vector<DataStat> struct_data_stat;
        RendererColor rend_color;

        struct PairHash {
            std::size_t operator() (const std::pair<Stop*, Stop*>& p) const {
                auto h1 = std::hash<const void*>{}(p.first);
                auto h2 = std::hash<const void*>{}(p.second);
                return h1 + h2;
            }
        };
	};
}
