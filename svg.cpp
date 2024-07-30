#include "svg.h"

namespace svg {

	using namespace std::literals;

	// ---------- Color ------------------

	std::ostream& operator<<(std::ostream& os, const StrokeLineCap& cap) {
		switch (cap) {
		case StrokeLineCap::BUTT:      os << "butt"; break;
		case StrokeLineCap::ROUND:     os << "round"; break;
		case StrokeLineCap::SQUARE:    os << "square"; break;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const StrokeLineJoin& join) {
		switch (join) {
		case StrokeLineJoin::ARCS:       os << "arcs"; break;
		case StrokeLineJoin::BEVEL:      os << "bevel"; break;
		case StrokeLineJoin::MITER:      os << "miter"; break;
		case StrokeLineJoin::MITER_CLIP: os << "miter-clip"; break;
		case StrokeLineJoin::ROUND:      os << "round"; break;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& out, Color& color) {
		std::visit(ColorPrinter{ out }, color);
		return out;
	}

	void Object::Render(const RenderContext& context) const {
		context.RenderIndent();

		RenderObject(context);
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

	void Circle::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<circle cx=\\\""sv << center_.x << "\\\" cy=\\\""sv << center_.y << "\\\" "sv;
		out << "r=\\\""sv << radius_ << "\\\""sv;
		RenderAttrs(context.out);
		out << "/>\\n"sv;
	}

	// ---------- Polyline ------------------

	Polyline& Polyline::AddPoint(Point point) {
		polylines_.push_back(std::move(point));
		return *this;
	}

	void Polyline::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<polyline points=\\\""sv;
		bool is_first = true;
		for (auto& point : polylines_) {
			if (is_first) {
				out << point.x << "," << point.y;
				is_first = false;
			}
			else {
				out << " "sv << point.x << "," << point.y;
			}
		}
		out << "\\\"";
		RenderAttrs(context.out);
		out << "/>\\n"sv;
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
		fontSize_ = size;
		return *this;
	}

	Text& Text::SetFontFamily(std::string font_family) {
		fontFamily_ = std::move(font_family);
		return *this;
	}

	Text& Text::SetFontWeight(std::string font_weight) {
		fontWeight_ = std::move(font_weight);
		return *this;
	}

	Text& Text::SetData(std::string data) {
		data_ = std::move(data);
		return *this;
	}

	void Text::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<text";
		RenderAttrs(context.out);
		out << " x=\\\""sv << position_.x << "\\\" y=\\\""sv << position_.y << "\\\" "sv;
		out << "dx=\\\""sv << offset_.x << "\\\" dy=\\\""sv << offset_.y << "\\\" "sv;
		out << "font-size=\\\""sv << fontSize_ << "\\\""sv;
		if (!fontFamily_.empty()) out << " font-family=\\\""sv << fontFamily_ << "\\\""sv;
		if (!fontWeight_.empty()) out << " font-weight=\\\""sv << fontWeight_ << "\\\""sv;
		out << ">"sv << data_ << "</text>\\n"sv;
	}

	// ---------- Document ------------------

	void Document::AddPtr(std::unique_ptr<Object>&& obj) {
		objects_.emplace_back(std::move(obj));
	}

	void Document::Render(std::ostream& out) const {
		RenderContext ctx(out, 2, 2);
		out << "<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" ?>\\n"sv;
		out << "<svg xmlns=\\\"http://www.w3.org/2000/svg\\\" version=\\\"1.1\\\">\\n"sv;
		for (const auto& obj : objects_) {
			obj->Render(ctx);
		}
		out << "</svg>"sv;
	}

}  // namespace svg