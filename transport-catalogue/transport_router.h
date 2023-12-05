#pragma once

#include <optional>

#include "transport_catalogue.h"
#include "graph.h"
#include "router.h"

using namespace std;

namespace router {
	class TransportRouter : catalogue::TransportCatalogue {
	public:
		/*�������� ������ domain_, ��� ��� ����� ��� ���� � ��� ���������, ���� ����������� ��� � ����� json_reader. ������ � �� ��������� ���,
		��� ��� ����������� domain ������ ��� ���������� �������� � �� ��������, �� ������ ��� ����� ����������� � json_reader'e ������.*/
		TransportRouter(domain::Domain& domain, catalogue::TransportCatalogue& catalog, graph::DirectedWeightedGraph<double>& graph) : domain_(domain), catalog_(catalog), graph_(graph), router_(make_unique<graph::Router<double>>(graph_)) {
			FillGraph();
			graph::Router<double> router(graph_);
			router_ = make_unique<graph::Router<double>>(router);
		}
		RouteInfo GetRouterInfo(string_view from, string_view to);

	private:
		void FillGraph();

		domain::Domain& domain_;
		catalogue::TransportCatalogue& catalog_;
		graph::DirectedWeightedGraph<double> graph_;
		unique_ptr<graph::Router<double>> router_;
		unordered_map<EdgeData, string, EdgeDataHash> edges;
	};
}