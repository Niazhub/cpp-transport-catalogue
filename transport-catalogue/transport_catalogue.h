#pragma once

#include <iostream>
#include <algorithm>
#include <deque>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>
#include "geo.h"
#include "domain.h"

namespace catalogue {
    class TransportCatalogue : public virtual domain::Domain{
    public:
        void AddStop(const Stop& stop);
        void AddStopDistance(std::string_view from, std::string_view to, const double& distance);
        void AddBus(const Bus& bus);
        Stop* FindStop(std::string_view stop);
        Bus* FindBus(std::string_view bus);
        BusInfo GetBusInfo(std::string_view bus);
        StopInfo GetStopInfo(std::string_view stop);
        double GetDistance(std::string_view from, std::string_view to);
        std::vector<Bus> GetBuses();

    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, Stop*> hash_table_stops_;
        std::unordered_map<std::string_view, std::set<std::string>> stops_to_bus_;
        std::unordered_map<std::pair<Stop*, Stop*>, double, PairHash> stops_distance_;

        std::deque<Bus> buses_;
        std::unordered_map<std::string_view, Bus*> hash_table_buses_;
    };
}