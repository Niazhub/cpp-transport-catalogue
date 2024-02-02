#include "transport_router.h"

void router::TransportRouter::FillGraph() {
	for (const auto& bus : catalog_.GetHashTableBuses()) {
		for (size_t i = 0; i < bus.second->bus_to_stops_.size() - 1; ++i) {
			for (size_t j = i + 1; j < bus.second->bus_to_stops_.size(); ++j) {
				double dis = catalog_.GetLongDistance(i, j, bus.second);
				double weight = dis / 1000 / domain_.routing_settings.bus_velocity * 60 + domain_.routing_settings.bus_wait_time;
				graph_->AddEdge({
					catalog_.FindStopToSizeT(bus.second->bus_to_stops_[i]),
					catalog_.FindStopToSizeT(bus.second->bus_to_stops_[j]),
					weight
					});
				edges.insert({
					{
						catalog_.FindStopToSizeT(bus.second->bus_to_stops_[i]),
						catalog_.FindStopToSizeT(bus.second->bus_to_stops_[j]),
						weight
					},
					bus.second->bus_name_
					});
			}
		}
	}
}

optional<domain::Domain::RouteInfo> router::TransportRouter::GetRouterInfo(string_view from, string_view to) {
	size_t value_from = catalog_.FindStopToSizeT(catalog_.FindStop(from));
	size_t value_to = catalog_.FindStopToSizeT(catalog_.FindStop(to));
	if (value_from == std::numeric_limits<size_t>::max() || value_to == std::numeric_limits<size_t>::max()) {
		RouteInfo route_info;
		route_info.error = "not found";
		stat_route_info = move(route_info);
		return stat_route_info;
	}

	optional<graph::Router<double>::RouteInfo> result = router_->BuildRoute(value_from, value_to);

	if (!result.has_value()) {
		RouteInfo route_info;
		route_info.error = "not found";
		stat_route_info = move(route_info);
		return stat_route_info;
	}

	double time = 0.0;
	optional<RouteInfo> route_info;
	route_info.emplace();

	for (const auto& edge : result.value().edges) {
		Stop* stop_from = catalog_.FindSizeTToStop(graph_->GetEdge(edge).from);
		Stop* stop_to = catalog_.FindSizeTToStop(graph_->GetEdge(edge).to);

		string bus_name = edges.at({ graph_->GetEdge(edge).from, graph_->GetEdge(edge).to, graph_->GetEdge(edge).weight });

		Bus* bus = catalog_.FindBus(bus_name);
		vector<int> vector_counts;
		int spand_count = 0;
		int count = 0;
		string is_inside = "";

		if (!bus->is_roundtrip) {
			for (int i = 0; i < bus->bus_to_stops_.size(); ++i) {
				if ((stop_to == bus->bus_to_stops_[i] || stop_from == bus->bus_to_stops_[i])) {
					if (is_inside != bus->bus_to_stops_[i]->stop_name_) {
						vector_counts.push_back(spand_count);
					}
					is_inside = bus->bus_to_stops_[i]->stop_name_;
					spand_count = 0;
				}
				if (!is_inside.empty()) {
					spand_count++;
				}
			}
			vector_counts.erase(std::remove(vector_counts.begin(), vector_counts.end(), 0), vector_counts.end());
			auto min_it = min_element(vector_counts.begin(), vector_counts.end());
			count = min_it == vector_counts.end() ? 0 : *min_it;
		}
		else {
			for (int i = 0; i < bus->bus_to_stops_.size(); ++i) {
				if (stop_from == bus->bus_to_stops_[i]) {
					is_inside = bus->bus_to_stops_[i]->stop_name_;
					spand_count = 0;
				}
				if (stop_to == bus->bus_to_stops_[i]) {
					vector_counts.push_back(spand_count);
					is_inside = "";
					spand_count = 0;
				}
				if (!is_inside.empty()) {
					spand_count++;
				}
			}
			vector_counts.erase(std::remove(vector_counts.begin(), vector_counts.end(), 0), vector_counts.end());
			auto min_it = min_element(vector_counts.begin(), vector_counts.end());
			count = min_it == vector_counts.end() ? 0 : *min_it;
		}

		route_info->stat_route_output.push_back(RouteOutPut{ "Wait" , stop_from->stop_name_, "", 0 , static_cast<double>(domain_.routing_settings.bus_wait_time) });
		route_info->stat_route_output.push_back(RouteOutPut{ "Bus" , "", bus_name, count, graph_->GetEdge(edge).weight - domain_.routing_settings.bus_wait_time });
		time += graph_->GetEdge(edge).weight;
	}

	route_info->total_time = move(time);
	stat_route_info = route_info.value();

	return stat_route_info;
}