#pragma once

#include <optional>

#include "transport_catalogue.h"
#include "graph.h"
#include "router.h"

using namespace std;

namespace router {
	class TransportRouter : catalogue::TransportCatalogue {
	public:
		TransportRouter(domain::Domain& domain, catalogue::TransportCatalogue& catalog) : domain_(domain), catalog_(catalog) {}
		graph::DirectedWeightedGraph<double> AddGraph();
		RouteInfo GetRouterInfo(const graph::DirectedWeightedGraph<double>& graph, const graph::Router<double>& router, const DataStat& value);

	private:
		void ParseEdges(graph::DirectedWeightedGraph<double>& graph);

		domain::Domain& domain_;
		catalogue::TransportCatalogue& catalog_;
		unordered_map<EdgeData, string, EdgeDataHash> edges;
	};
}