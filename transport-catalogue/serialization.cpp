#include "serialization.h"

void serialization::Serialization(std::ostream& file, const catalogue::TransportCatalogue& catalogue_, domain::Domain& domain) {
    serialization_catalogue::CatalogueList catalogue_list;

    AddBuses(catalogue_list, catalogue_);
    AddStops(catalogue_list, catalogue_);
    AddColorSettings(catalogue_list, domain);
    AddRouter(catalogue_list, domain);

    catalogue_list.SerializeToOstream(&file);
}

void serialization::Deserialization(std::istream& file, catalogue::TransportCatalogue& catalogue_, domain::Domain& domain) {
    serialization_catalogue::CatalogueList catalogue_list;

    catalogue_list.ParseFromIstream(&file);
    
    GetStops(catalogue_list, catalogue_);
    GetBuses(catalogue_list, catalogue_);
    GetColorSettings(catalogue_list, domain);
    for (const auto& value : catalogue_.GetHashTableStops()) {
        for (const auto& val : value.second->road_distances) {
            catalogue_.AddStopDistance(value.first, val.first, val.second);
        }
    }
    GetRouter(catalogue_, domain, catalogue_list);
}

void serialization::AddBuses(serialization_catalogue::CatalogueList& catalogue_list, const catalogue::TransportCatalogue& catalogue_) {
    for (const auto& bus : catalogue_.GetBuses()) {
 	    auto* curr_bus = catalogue_list.add_buses();
 	    curr_bus->set_name(bus.bus_name_);
 	    curr_bus->set_roundtrip(bus.is_roundtrip);

 	    for (const auto& stop : bus.bus_to_stops_) {
 		    auto* curr_stop = curr_bus->add_stops();
 		    curr_stop->set_name(stop->stop_name_);
 		    curr_stop->set_lat(stop->stop_coordinates_.lat);
 		    curr_stop->set_lng(stop->stop_coordinates_.lng);
 	    }
    }
}

void serialization::AddStops(serialization_catalogue::CatalogueList & catalogue_list, const catalogue::TransportCatalogue & catalogue_) {
    for (const auto& stop : catalogue_.GetStops()) {
 	    auto* curr_stop = catalogue_list.add_stops();
 	    curr_stop->set_name(stop.stop_name_);
 	    curr_stop->set_lat(stop.stop_coordinates_.lat);
 	    curr_stop->set_lng(stop.stop_coordinates_.lng);

 	    auto* curr_distance = curr_stop->mutable_road_distance();
 	    *curr_distance = { stop.road_distances.begin(), stop.road_distances.end() };
    }
}

void serialization::AddColorSettings(serialization_catalogue::CatalogueList& catalogue_list, const domain::Domain& domain) {
    auto* cuur_color = catalogue_list.mutable_rend_color();
    cuur_color->set_width(domain.rend_color.width);
    cuur_color->set_height(domain.rend_color.height);
    cuur_color->set_padding(domain.rend_color.padding);
    cuur_color->set_line_width(domain.rend_color.line_width);
    cuur_color->set_stop_radius(domain.rend_color.stop_radius);
    cuur_color->set_bus_label_font_size(domain.rend_color.bus_label_font_size);

    auto* curr_bus_offset = cuur_color->mutable_bus_label_offset();
    for (const auto& val : domain.rend_color.bus_label_offset) {
 	    *curr_bus_offset->Add() = val;
    }

    cuur_color->set_stop_label_font_size(domain.rend_color.stop_label_font_size);

    auto* curr_stop_offset = cuur_color->mutable_stop_label_offset();
    for (const auto& val : domain.rend_color.stop_label_offset) {
 	    *curr_stop_offset->Add() = val;
    }

    auto* curr_underlayer_color = cuur_color->mutable_underlayer_color();
    if (std::holds_alternative<std::string>(domain.rend_color.underlayer_color)) {
 	    std::string color = std::get<std::string>(domain.rend_color.underlayer_color);
 	    curr_underlayer_color->set_color(color);
    }
    else if (std::holds_alternative<svg::Rgb>(domain.rend_color.underlayer_color)) {
 	    svg::Rgb color = std::get<svg::Rgb>(domain.rend_color.underlayer_color);
 	    curr_underlayer_color->set_red(color.red);
 	    curr_underlayer_color->set_green(color.green);
 	    curr_underlayer_color->set_blue(color.blue);
 	    curr_underlayer_color->set_opacity(-1);
    }
    else if (std::holds_alternative<svg::Rgba>(domain.rend_color.underlayer_color)) {
 	    svg::Rgba color = std::get<svg::Rgba>(domain.rend_color.underlayer_color);
 	    curr_underlayer_color->set_red(color.red);
 	    curr_underlayer_color->set_green(color.green);
 	    curr_underlayer_color->set_blue(color.blue);
 	    curr_underlayer_color->set_opacity(color.opacity);
    }

    cuur_color->set_underlayer_width(domain.rend_color.underlayer_width);

    for (const auto& val : domain.rend_color.color_palette) {
 	    auto* curr_color_palette = cuur_color->add_color_palette();
 	    if (std::holds_alternative<std::string>(val)) {
 		    std::string color = std::get<std::string>(val);
 		    curr_color_palette->set_color(color);
 	    }
 	    else if (std::holds_alternative<svg::Rgb>(val)) {
 		    svg::Rgb color = std::get<svg::Rgb>(val);
 		    curr_color_palette->set_red(color.red);
 		    curr_color_palette->set_green(color.green);
 		    curr_color_palette->set_blue(color.blue);
            curr_color_palette->set_opacity(-1);
 	    }
 	    else if (std::holds_alternative<svg::Rgba>(val)) {
 		    svg::Rgba color = std::get<svg::Rgba>(val);
 		    curr_color_palette->set_red(color.red);
 		    curr_color_palette->set_green(color.green);
 		    curr_color_palette->set_blue(color.blue);
 		    curr_color_palette->set_opacity(color.opacity);
 	    }
    }
}

void serialization::AddRouter(serialization_catalogue::CatalogueList& catalogue_list, domain::Domain& domain) {
    auto* curr_routing_settings = catalogue_list.mutable_routing_settings();
    curr_routing_settings->set_bus_velocity(domain.routing_settings.bus_velocity);
    curr_routing_settings->set_bus_wait_time(domain.routing_settings.bus_wait_time);
}

void serialization::GetBuses(const serialization_catalogue::CatalogueList & catalogue_list, catalogue::TransportCatalogue& catalogue_) {
    for (const auto& bus : catalogue_list.buses()) {
 	    std::vector<catalogue::TransportCatalogue::Stop*> stops;

 	    for (const auto& val : bus.stops()) {
 		    if (auto it = catalogue_.FindStop(val.name()); it != nullptr) {
 			    stops.push_back(it);
 		    }
 	    }
 	    catalogue_.AddBus({ bus.name(), stops, bus.roundtrip() });
    }
}

void serialization::GetStops(const serialization_catalogue::CatalogueList & catalogue_list, catalogue::TransportCatalogue& catalogue_) {
    for (const auto& stop : catalogue_list.stops()) {
 	    std::unordered_map<std::string, double> curr_road_distance = { stop.road_distance().begin(), stop.road_distance().end() };
 	    catalogue_.AddStop({ stop.name(), { stop.lat(), stop.lng() }, curr_road_distance });
    }
}

void serialization::GetColorSettings(const serialization_catalogue::CatalogueList& catalogue_list, domain::Domain& domain) {
    auto curr_color = catalogue_list.rend_color();
    domain.rend_color.width = curr_color.width();
    domain.rend_color.height = curr_color.height();
    domain.rend_color.padding = curr_color.padding();
    domain.rend_color.line_width = curr_color.line_width();
    domain.rend_color.stop_radius = curr_color.stop_radius();
    domain.rend_color.bus_label_font_size = curr_color.bus_label_font_size();

    for (const auto& val : curr_color.bus_label_offset()) {
        domain.rend_color.bus_label_offset.push_back(val);
    }
    domain.rend_color.stop_label_font_size = curr_color.stop_label_font_size();
    for (const auto& val : curr_color.stop_label_offset()) {
        domain.rend_color.stop_label_offset.push_back(val);
    }

    if (!curr_color.underlayer_color().color().empty()) {
        domain.rend_color.underlayer_color = curr_color.underlayer_color().color();
    }
    else if (curr_color.underlayer_color().opacity() != -1) {
        domain.rend_color.underlayer_color = svg::Rgba(static_cast<uint8_t>(curr_color.underlayer_color().red()),
 																    static_cast<uint8_t>(curr_color.underlayer_color().green()),
 																    static_cast<uint8_t>(curr_color.underlayer_color().blue()),
                                                                    curr_color.underlayer_color().opacity());
    }
    else {
        domain.rend_color.underlayer_color = svg::Rgb(static_cast<uint8_t>(curr_color.underlayer_color().red()),
 											    static_cast<uint8_t>(curr_color.underlayer_color().green()),
 											    static_cast<uint8_t>(curr_color.underlayer_color().blue()));
    }

    domain.rend_color.underlayer_width = curr_color.underlayer_width();

    for (const auto& val : curr_color.color_palette()) {
 	    if (!val.color().empty()) {
            domain.rend_color.color_palette.push_back(val.color());
 	    }
 	    else if (val.opacity() != -1) {
            domain.rend_color.color_palette.push_back(svg::Rgba(static_cast<uint8_t>(val.red()),
                                                                static_cast<uint8_t>(val.green()),
                                                                static_cast<uint8_t>(val.blue()),
                                                                val.opacity()));
 	    }
 	    else {
            domain.rend_color.color_palette.push_back(svg::Rgb(static_cast<uint8_t>(val.red()),
                                                                static_cast<uint8_t>(val.green()),
                                                                static_cast<uint8_t>(val.blue())));
 	    }
    }
}

void serialization::GetRouter(catalogue::TransportCatalogue& catalogue_, domain::Domain& domain, const serialization_catalogue::CatalogueList& catalogue_list) {
    domain.routing_settings.bus_velocity = catalogue_list.routing_settings().bus_velocity();
    domain.routing_settings.bus_wait_time = catalogue_list.routing_settings().bus_wait_time();
}