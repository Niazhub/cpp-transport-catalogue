#include "svg.h"

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        RenderObject(context);

        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    Circle& Circle::SetFill(Color fill_color) {
        return SetFillColor(fill_color);
    }

    Circle& Circle::SetStroke(Color stroke_color) {
        return SetStrokeColor(stroke_color);
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\" "sv;
        RenderAttrs(context.out);
        out << "/>"sv;
    }

    // ---------- Polyline ------------------

    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }

    Polyline& Polyline::SetFill(Color fill_color) {
        return SetFillColor(fill_color);
    }

    Polyline& Polyline::SetStroke(Color stroke_color) {
        return SetStrokeColor(stroke_color);
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <polyline points=\"";
        for (size_t i = 0; i < points_.size(); ++i) {
            out << points_[i].x << "," << points_[i].y;
            if (i != points_.size() - 1) {
                out << " ";
            }
        }
        out << "\" ";
        RenderAttrs(context.out);
        out << "/>";
    }

    // ---------- Text ------------------

    Text& Text::SetPosition(Point pos) {
        position_ = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        font_size_ = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = std::move(font_family);
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = std::move(font_weight);
        return *this;
    }

    Text& Text::SetData(std::string data) {
        data_ = std::move(data);
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <text ";
        RenderAttrs(context.out);
        out << " x=\"" << position_.x << "\" y=\"" << position_.y << "\" ";

        out << "dx=\"" << offset_.x << "\" dy=\"" << offset_.y << "\" ";

        out << "font-size=\"" << font_size_ << "\" ";

        if (!font_family_.empty()) {
            context.out << "font-family=\"" << font_family_ << "\"";
        }

        if (!font_weight_.empty()) {
            context.out << " font-weight=\"" << font_weight_ << "\"";
        }
        out << ">" << data_ << "</text>";
    }

    // ---------- Document ------------------

    void Document::AddPtr(std::unique_ptr<Object>&& object_ptr) {
        objects_.emplace_back(std::move(object_ptr));
    }

    void Document::Render(std::ostream& out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;

        for (const auto& object : objects_) {
            RenderContext context(out);
            object->Render(context);
        }

        out << "</svg>";
    }

    std::ostream& operator<<(std::ostream& out, const StrokeLineCap& cap) {
        switch (cap) {
        case StrokeLineCap::BUTT:
            out << "butt"sv;
            break;
        case StrokeLineCap::ROUND:
            out << "round"sv;
            break;
        case StrokeLineCap::SQUARE:
            out << "square"sv;
            break;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const StrokeLineJoin& join) {
        switch (join) {
        case StrokeLineJoin::ARCS:
            out << "arcs"sv;
            break;
        case StrokeLineJoin::BEVEL:
            out << "bevel"sv;
            break;
        case StrokeLineJoin::MITER:
            out << "miter"sv;
            break;
        case StrokeLineJoin::MITER_CLIP:
            out << "miter-clip"sv;
            break;
        case StrokeLineJoin::ROUND:
            out << "round"sv;
            break;
        }
        return out;
    }

    void PrintColor::operator()(const std::monostate&) {
        out << "none";
    }

    void PrintColor::operator()(const std::string& str) {
        out << str;
    }

    void PrintColor::operator()(const Rgb& rgb) {
        out << "rgb(" << int(rgb.red) << "," << int(rgb.green) << "," << int(rgb.blue) << ")";
    }

    void PrintColor::operator()(const Rgba& rgba) {
        out << "rgba(" << int(rgba.red) << "," << int(rgba.green) << "," << int(rgba.blue) << "," << rgba.opacity << ")";
    }

}  // namespace svg