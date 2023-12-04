#pragma once

#include <iostream>
#include <algorithm>
#include <deque>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <functional>
#include "geo.h"
#include "domain.h"

namespace catalogue {
    class TransportCatalogue : public virtual domain::Domain{
    public:
        void AddStop(const Stop& stop);
        void AddStopDistance(std::string_view from, std::string_view to, double distance);
        void AddBus(const Bus& bus);
        Stop* FindStop(std::string_view stop) const;
        Bus* FindBus(std::string_view bus) const;
        Bus* FindBus(Stop* stop) const;
        size_t FindStopToSizeT(Stop* stop) const;
        Stop* FindSizeTToStop(size_t id) const;
        BusInfo GetBusInfo(std::string_view bus) const;
        StopInfo GetStopInfo(std::string_view stop) const;
        double GetDistance(std::string_view from, std::string_view to) const;
        double GetLongDistance(size_t from, size_t to, Bus* bus) const;
        std::vector<Bus> GetBuses() const;
        std::unordered_map<std::string_view, Bus*> GetHashTableBuses() const;
        std::unordered_map<std::string_view, Stop*> GetHashTableStops() const;

    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, Stop*> hash_table_stops_;
        std::unordered_map<std::string_view, std::set<std::string>> stops_to_bus_;
        std::unordered_map<std::pair<Stop*, Stop*>, double, PairHash> stops_distance_;
        std::unordered_map<Stop*, size_t> stop_to_size_t;
        std::unordered_map<size_t, Stop*> size_t_to_stop;

        std::deque<Bus> buses_;
        std::unordered_map<std::string_view, Bus*> hash_table_buses_;
    };
}