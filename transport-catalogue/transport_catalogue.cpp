#include "transport_catalogue.h"
#include "map_renderer.h"

catalogue::TransportCatalogue::Stop* catalogue::TransportCatalogue::FindStop(std::string_view stop) {
	auto it = hash_table_stops_.find(stop);
	if (it != hash_table_stops_.end()) {
		return it->second;
	}
	return nullptr;
}

catalogue::TransportCatalogue::Bus* catalogue::TransportCatalogue::FindBus(std::string_view bus) {
	auto it = hash_table_buses_.find(bus);
	if (it != hash_table_buses_.end()) {
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

void catalogue::TransportCatalogue::AddStopDistance(std::string_view from, std::string_view to, const double& distance) {
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
	for (Stop* stop : buses_.back().bus_to_stops_) {
		stops_to_bus_[stop->stop_name_].insert(buses_.back().bus_name_);
	}
}

catalogue::TransportCatalogue::BusInfo catalogue::TransportCatalogue::GetBusInfo(std::string_view bus_name) {
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

catalogue::TransportCatalogue::StopInfo catalogue::TransportCatalogue::GetStopInfo(std::string_view stop) {
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

double catalogue::TransportCatalogue::GetDistance(std::string_view from, std::string_view to) {
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

std::vector<domain::Domain::Bus> catalogue::TransportCatalogue::GetBuses() {
	std::vector<Bus> buses;
	for (const auto& value : buses_) {
		buses.push_back(value);
	}
	return buses;
}