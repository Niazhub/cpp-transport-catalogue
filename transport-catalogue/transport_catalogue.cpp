#include "transport_catalogue.h"
#include "map_renderer.h"

catalogue::TransportCatalogue::Stop* catalogue::TransportCatalogue::FindStop(std::string_view stop) const{
	auto it = hash_table_stops_.find(stop);
	if (it != hash_table_stops_.end()) {
		return it->second;
	}
	return nullptr;
}

catalogue::TransportCatalogue::Bus* catalogue::TransportCatalogue::FindBus(std::string_view bus) const{
	auto it = hash_table_buses_.find(bus);
	if (it != hash_table_buses_.end()) {
		return it->second;
	}
	return nullptr;
}

catalogue::TransportCatalogue::Bus* catalogue::TransportCatalogue::FindBus(Stop* stop) const {
	for (const auto& bus : hash_table_buses_) {
		auto it = find(bus.second->bus_to_stops_.begin(), bus.second->bus_to_stops_.end(), stop);
		if (it != bus.second->bus_to_stops_.end()) {
			return bus.second;
		}
	}
	return nullptr;
}

size_t catalogue::TransportCatalogue::FindStopToSizeT(Stop* stop) const {
	auto it = stop_to_size_t.find(stop);
	if (it != stop_to_size_t.end()) {
		return it->second;
	}
	return std::numeric_limits<size_t>::max();
}

catalogue::TransportCatalogue::Stop* catalogue::TransportCatalogue::FindSizeTToStop(size_t id) const {
	auto it = size_t_to_stop.find(id);
	if (it != size_t_to_stop.end()) {
		return it->second;
	}
	return nullptr;
}

void catalogue::TransportCatalogue::AddStop(const Stop& stop) {
	auto it = hash_table_stops_.find(stop.stop_name_);
	if (it == hash_table_stops_.end()) {
		stops_.push_back(stop);
		hash_table_stops_[stops_.back().stop_name_] = &stops_.back();
	}
}

void catalogue::TransportCatalogue::AddStopDistance(std::string_view from, std::string_view to, double distance) {
	Stop* stop_from = FindStop(from);
	Stop* stop_to = FindStop(to);
	if (stop_from && stop_to) {
		stops_distance_.insert({ { stop_from, stop_to }, distance });
	}
}

void catalogue::TransportCatalogue::AddBus(const Bus& bus) {
	auto it = hash_table_buses_.find(bus.bus_name_);
	if (it == hash_table_buses_.end()) {
		buses_.push_back(bus);
		hash_table_buses_[buses_.back().bus_name_] = &buses_.back();
	}
	for (int i = 0; i < buses_.back().bus_to_stops_.size(); ++i) {
		stops_to_bus_[buses_.back().bus_to_stops_[i]->stop_name_].insert(buses_.back().bus_name_);
		stop_to_size_t.insert({ buses_.back().bus_to_stops_[i], stop_to_size_t.size()});
		size_t_to_stop.insert({ stop_to_size_t.size() - 1, buses_.back().bus_to_stops_[i] });
	}
}

catalogue::TransportCatalogue::BusInfo catalogue::TransportCatalogue::GetBusInfo(std::string_view bus_name) const{
	Bus* bus = FindBus(bus_name);
	if (bus == nullptr) {
		return { 0, 0, 0.0, 0.0 };
	}

	BusInfo busInfo;

	double distance = 0;
	for (size_t i = 0; i < bus->bus_to_stops_.size(); ++i) {
		busInfo.total_stops++;
		distance += geo::ComputeDistance(bus->bus_to_stops_[i]->stop_coordinates_,
			bus->bus_to_stops_[(i + 1) % bus->bus_to_stops_.size()]->stop_coordinates_);
		busInfo.route_length += GetDistance(bus->bus_to_stops_[i]->stop_name_, bus->bus_to_stops_[(i + 1) % bus->bus_to_stops_.size()]->stop_name_);
		if (std::find(bus->bus_to_stops_.begin(), bus->bus_to_stops_.begin() + i, bus->bus_to_stops_[i])
			== bus->bus_to_stops_.begin() + i) {
			busInfo.unique_stops++;
		}
	}

	busInfo.curvature = busInfo.route_length / distance;

	return { busInfo.total_stops, busInfo.unique_stops, busInfo.route_length, busInfo.curvature };
}

catalogue::TransportCatalogue::StopInfo catalogue::TransportCatalogue::GetStopInfo(std::string_view stop) const{
	auto it = hash_table_stops_.find(stop);
	if (it != hash_table_stops_.end()) {
		auto it_1 = stops_to_bus_.find(stop);
		if (it_1 != stops_to_bus_.end()) {
			return { it_1->second, true };
		}
		else {
			return { {}, true };
		}
	}
	else {
		return { {}, false };
	}
}

double catalogue::TransportCatalogue::GetDistance(std::string_view from, std::string_view to) const {
	Stop* stop_from = FindStop(from);
	Stop* stop_to = FindStop(to);
	auto it = stops_distance_.find({ stop_from, stop_to });
	if (it != stops_distance_.end()) {
		return it->second;
	}
	else {
		auto it1 = stops_distance_.find({ stop_to, stop_from });
		if (it1 != stops_distance_.end()) {
			return it1->second;
		}
	}
	return 0.0;
}

double catalogue::TransportCatalogue::GetLongDistance(size_t from, size_t to, Bus* bus) const {
	double dis = 0.0;
	for (size_t i = from; i < to; ++i) {
		auto it = stops_distance_.find({ bus->bus_to_stops_[i], bus->bus_to_stops_[i+1] });
		if (it != stops_distance_.end()) {
			dis += it->second;
		}
		else {
			auto it1 = stops_distance_.find({ bus->bus_to_stops_[i+1], bus->bus_to_stops_[i] });
			if (it1 != stops_distance_.end()) {
				dis += it1->second;
			}
		}
	}
	return dis;
}

std::vector<domain::Domain::Bus> catalogue::TransportCatalogue::GetBuses() const{
	std::vector<Bus> buses;
	for (const auto& value : buses_) {
		buses.push_back(value);
	}
	return buses;
}

std::vector<domain::Domain::Stop> catalogue::TransportCatalogue::GetStops() const {
	std::vector<Stop> stops;
	for (const auto& value : stops_) {
		stops.push_back(value);
	}
	return stops;
}

std::unordered_map<std::string_view, domain::Domain::Bus*> catalogue::TransportCatalogue::GetHashTableBuses() const {
	return hash_table_buses_;
}

std::unordered_map<std::string_view, domain::Domain::Stop*> catalogue::TransportCatalogue::GetHashTableStops() const {
	return hash_table_stops_;
}