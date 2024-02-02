#pragma once

#include <transport_catalogue.pb.h>
#include <fstream>
#include <variant>
#include <iostream>
#include "domain.h"
#include "transport_router.h"

namespace serialization {
	void Serialization(std::ostream& file, const catalogue::TransportCatalogue& catalogue_, domain::Domain& domain);
	
	void Deserialization(std::istream& file, catalogue::TransportCatalogue& catalogue_, domain::Domain& domain);

	void AddBuses(serialization_catalogue::CatalogueList& catalogue_list, const catalogue::TransportCatalogue& catalogue_);

	void AddStops(serialization_catalogue::CatalogueList& catalogue_list, const catalogue::TransportCatalogue& catalogue_);

	void AddColorSettings(serialization_catalogue::CatalogueList& catalogue_list, const domain::Domain& domain);

	void AddRouter(serialization_catalogue::CatalogueList& catalogue_list, domain::Domain& domain);

	void GetBuses(const serialization_catalogue::CatalogueList& catalogue_list, catalogue::TransportCatalogue& catalogue_);

	void GetStops(const serialization_catalogue::CatalogueList& catalogue_list, catalogue::TransportCatalogue& catalogue_);

	void GetColorSettings(const serialization_catalogue::CatalogueList& catalogue_list, domain::Domain& domain);

	void GetRouter(catalogue::TransportCatalogue& catalogue_, domain::Domain& domain, const serialization_catalogue::CatalogueList& catalogue_list);
}