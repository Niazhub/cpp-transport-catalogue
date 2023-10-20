#include "map_renderer.h"

bool renderer::IsZero(double value) {
    return std::abs(value) < EPSILON;
}

svg::Point renderer::SphereProjector::operator()(geo::Coordinates coords) const {
    return {
        (coords.lng - min_lon_) * zoom_coeff_ + padding_,
        (max_lat_ - coords.lat) * zoom_coeff_ + padding_
    };
}

void renderer::MapRenderer::SetBuses(domain::Domain::Bus buses) {
    buses_.push_back(buses);
}

void renderer::MapRenderer::SetColorSettings(RendererColor rend_color) {
    rend_color_ = rend_color;
}

void renderer::MapRenderer::RenderPolylineStops(svg::Document& document, SphereProjector& sphere) {
    int i = 0;
    for (const auto& value : buses_) {
        svg::Polyline polyline;
        if (rend_color_.color_palette.size() <= static_cast<std::vector<svg::Color>::size_type>(i)) {
            i = 0;
        }
        for (const auto& value_1 : value.bus_to_stops_) {
            polyline.AddPoint({ sphere(value_1->stop_coordinates_) });
            polyline.SetFill(svg::NoneColor);
            polyline.SetStroke(rend_color_.color_palette[i]);
            polyline.SetStrokeWidth(rend_color_.line_width);
            polyline.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
            polyline.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
        }
        document.Add(polyline);
        i++;
    }
}

void renderer::MapRenderer::RenderBusesName(svg::Document& document, SphereProjector& sphere) {
    int j = 0;
    for (const auto& value : buses_) {
        if (rend_color_.color_palette.size() <= static_cast<std::vector<svg::Color>::size_type>(j)) {
            j = 0;
        }
        std::vector<std::string> vec;
        for (size_t i = 0; i < value.bus_to_stops_.size(); ++i) {
            if (value.is_roundtrip == true) {
                if (i == 0) {
                    svg::Text text_1;
                    text_1.SetFillColor(rend_color_.underlayer_color);
                    text_1.SetStrokeColor(rend_color_.underlayer_color);
                    text_1.SetStrokeWidth(rend_color_.underlayer_width);
                    text_1.SetFontSize(rend_color_.bus_label_font_size);
                    text_1.SetFontFamily("Verdana");
                    text_1.SetFontWeight("bold");
                    text_1.SetPosition(sphere(value.bus_to_stops_[i]->stop_coordinates_));
                    text_1.SetOffset({ rend_color_.bus_label_offset[0], rend_color_.bus_label_offset[1] });
                    text_1.SetData(value.bus_name_);
                    text_1.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
                    text_1.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
                    document.Add(text_1);
                    svg::Text text_2;
                    text_2.SetFillColor(rend_color_.color_palette[j]);
                    text_2.SetPosition(sphere(value.bus_to_stops_[i]->stop_coordinates_));
                    text_2.SetOffset({ rend_color_.bus_label_offset[0], rend_color_.bus_label_offset[1] });
                    text_2.SetFontSize(rend_color_.bus_label_font_size);
                    text_2.SetFontFamily("Verdana");
                    text_2.SetFontWeight("bold");
                    text_2.SetData(value.bus_name_);
                    document.Add(text_2);
                }
            }
            else {
                if (i == 0 || i == value.bus_to_stops_.size() / 2) {
                    if (count(vec.begin(), vec.end(), value.bus_to_stops_[i]->stop_name_) == 0) {
                        svg::Text text_1;
                        text_1.SetFillColor(rend_color_.underlayer_color);
                        text_1.SetStrokeColor(rend_color_.underlayer_color);
                        text_1.SetStrokeWidth(rend_color_.underlayer_width);
                        text_1.SetFontSize(rend_color_.bus_label_font_size);
                        text_1.SetFontFamily("Verdana");
                        text_1.SetFontWeight("bold");
                        text_1.SetPosition(sphere(value.bus_to_stops_[i]->stop_coordinates_));
                        text_1.SetOffset({ rend_color_.bus_label_offset[0], rend_color_.bus_label_offset[1] });
                        text_1.SetData(value.bus_name_);
                        text_1.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
                        text_1.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
                        document.Add(text_1);
                        svg::Text text_2;
                        text_2.SetFillColor(rend_color_.color_palette[j]);
                        text_2.SetPosition(sphere(value.bus_to_stops_[i]->stop_coordinates_));
                        text_2.SetOffset({ rend_color_.bus_label_offset[0], rend_color_.bus_label_offset[1] });
                        text_2.SetFontSize(rend_color_.bus_label_font_size);
                        text_2.SetFontFamily("Verdana");
                        text_2.SetFontWeight("bold");
                        text_2.SetData(value.bus_name_);
                        document.Add(text_2);
                        vec.push_back(value.bus_to_stops_[i]->stop_name_);
                    }
                }
            }
        }
        j++;
    }
}

void renderer::MapRenderer::RenderCircleStops(svg::Document& document, SphereProjector& sphere, const std::vector<Stop*> sorted_stops_name) {
    svg::Circle circle;
    std::vector<std::string> vec_2;
    for (const auto& value : sorted_stops_name) {
        if (count(vec_2.begin(), vec_2.end(), value->stop_name_) == 0) {
            circle.SetCenter({ sphere(value->stop_coordinates_) });
            circle.SetFill("white");
            circle.SetRadius(rend_color_.stop_radius);
            document.Add(circle);
            vec_2.push_back(value->stop_name_);
        }
    }
}

void renderer::MapRenderer::RenderStopsName(svg::Document& document, SphereProjector& sphere, const std::vector<Stop*> sorted_stops_name) {
    std::vector<std::string> vec_3;
    for (auto value : sorted_stops_name) {
        svg::Text text_1;
        if (count(vec_3.begin(), vec_3.end(), value->stop_name_) == 0) {
            text_1.SetFillColor(rend_color_.underlayer_color);
            text_1.SetStrokeColor(rend_color_.underlayer_color);
            text_1.SetStrokeWidth(rend_color_.underlayer_width);
            text_1.SetFontSize(rend_color_.stop_label_font_size);
            text_1.SetFontFamily("Verdana");
            text_1.SetPosition(sphere(value->stop_coordinates_));
            text_1.SetOffset({ rend_color_.stop_label_offset[0], rend_color_.stop_label_offset[1] });
            text_1.SetData(value->stop_name_);
            text_1.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
            text_1.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
            document.Add(text_1);
            svg::Text text_2;
            text_2.SetFillColor("black");
            text_2.SetPosition(sphere(value->stop_coordinates_));
            text_2.SetOffset({ rend_color_.stop_label_offset[0], rend_color_.stop_label_offset[1] });
            text_2.SetFontSize(rend_color_.stop_label_font_size);
            text_2.SetFontFamily("Verdana");
            text_2.SetData(value->stop_name_);
            document.Add(text_2);
            vec_3.push_back(value->stop_name_);
        }
    }
}

void renderer::MapRenderer::RenderBuses(std::ostream& out) {
    std::sort(buses_.begin(), buses_.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.bus_name_ < rhs.bus_name_;
        });
    std::vector<geo::Coordinates> coordinates;
    for (const auto& value : buses_) {
        for (const auto& value_1 : value.bus_to_stops_) {
            coordinates.push_back(value_1->stop_coordinates_);
        }
    }
    svg::Document document;
    SphereProjector sphere(coordinates.begin(), coordinates.end(), rend_color_.width, rend_color_.height, rend_color_.padding);

    RenderPolylineStops(document, sphere);
    
    RenderBusesName(document, sphere);

    std::vector<Stop*> sorted_stops_name;
    for (const Bus& value : buses_) {
        for (Stop* value_1 : value.bus_to_stops_) {
            sorted_stops_name.push_back(value_1);
        }
    }

    std::sort(sorted_stops_name.begin(), sorted_stops_name.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->stop_name_ < rhs->stop_name_;
        });

    RenderCircleStops(document, sphere, sorted_stops_name);

    RenderStopsName(document, sphere, sorted_stops_name);

    document.Render(out);
}