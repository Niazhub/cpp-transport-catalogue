#pragma once

#include <optional>

#include "transport_catalogue.h"
#include "graph.h"
#include "router.h"

using namespace std;

namespace router {
	class TransportRouter : catalogue::TransportCatalogue {
	public:
		TransportRouter() = default;
		TransportRouter(const domain::Domain& domain, const catalogue::TransportCatalogue& catalog) 
			: domain_(domain),
			catalog_(catalog),
			graph_(make_unique<graph::DirectedWeightedGraph<double>>(catalog_.GetHashTableStops().size() * 2))
		{
			FillGraph();
			router_ = make_unique<graph::Router<double>>(*graph_);
		}

		optional<RouteInfo> GetRouterInfo(string_view from, string_view to);

	private:
		void FillGraph();

		domain::Domain domain_;
		catalogue::TransportCatalogue catalog_;
		unique_ptr<graph::DirectedWeightedGraph<double>> graph_;
		unique_ptr<graph::Router<double>> router_;
		unordered_map<EdgeData, string, EdgeDataHash> edges;
	};
}