#pragma once
#include "geo.h"
#include "svg.h"
#include "graph.h"
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
            std::unordered_map<std::string, double> road_distances;
        };

        struct Bus {
            std::string bus_name_;
            std::vector<Stop*> bus_to_stops_;
            bool is_roundtrip;

            Bus& operator++() {
                if (!bus_to_stops_.empty()) {
                    ++(bus_to_stops_.begin());
                }
                return *this;
            }
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

        struct RouteOutPut {
            std::string type;
            std::string stop_name;
            std::string bus;
            int span_count;
            double time;
        };

        struct RouteInfo {
            double total_time = 0.0;
            std::vector<RouteOutPut> stat_route_output;
            std::string error;
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
            std::string from;
            std::string to;
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

        struct RoutingSettings {
            int bus_wait_time;
            double bus_velocity;
        };

        struct EdgeData {
            size_t from;
            size_t to;
            double weight;

            bool operator==(const EdgeData& other) const {
                return from == other.from && to == other.to && weight == other.weight;
            }
        };
        
        std::vector<BusQueryes> struct_bus_base;
        std::vector<StopQueryes> struct_stop_base;
        std::vector<DataStat> struct_data_stat; 
        RouteInfo stat_route_info;
        RendererColor rend_color;
        RoutingSettings routing_settings;

        struct PairHash {
            std::size_t operator() (const std::pair<Stop*, Stop*>& p) const {
                auto h1 = std::hash<const void*>{}(p.first);
                auto h2 = std::hash<const void*>{}(p.second);
                return h1 + h2;
            }
        };

        struct EdgeDataHash {
            std::size_t operator() (const EdgeData& p) const {
                auto h1 = std::hash<size_t>{}(p.from);
                auto h2 = std::hash<size_t>{}(p.to);
                auto h3 = std::hash<double>{}(p.weight);
                return h1 + h2 + h3;
            }
        };
	};
}
