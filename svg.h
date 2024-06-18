#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace svg {

	enum class StrokeLineCap {
		BUTT,
		ROUND,
		SQUARE,
	};

	enum class StrokeLineJoin {
		ARCS,
		BEVEL,
		MITER,
		MITER_CLIP,
		ROUND,
	};

	std::ostream& operator<<(std::ostream& os, const StrokeLineCap& cap);
	std::ostream& operator<<(std::ostream& os, const StrokeLineJoin& join);

	struct Rgb {
		Rgb()
			: red(0)
			, green(0)
			, blue(0)
		{ }

		Rgb(uint8_t red, uint8_t green, uint8_t blue)
			: red(red)
			, green(green)
			, blue(blue)
		{ }

		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	struct Rgba : public Rgb {
		Rgba()
			: Rgb()
			, opacity(1.0) {
		}

		Rgba(uint8_t red, uint8_t green, uint8_t blue, double opacity)
			: Rgb(red, green, blue)
			, opacity(opacity) {
		}

		double opacity;
	};

	using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
	inline const Color NoneColor{ std::monostate() };

	std::ostream& operator<<(std::ostream& out, Color& color);

	struct ColorPrinter {
		std::ostream& out;
		void operator()(std::monostate) const { out << "none"; }
		void operator()(std::string color) const { out << color; }
		void operator()(Rgb color) const {
			out << "rgb("
				<< static_cast<int>(color.red) << "," << static_cast<int>(color.green) << "," << static_cast<int>(color.blue)
				<< ")";
		}
		void operator()(Rgba color) const {
			out << "rgba("
				<< static_cast<int>(color.red) << "," << static_cast<int>(color.green) << "," << static_cast<int>(color.blue) << "," << color.opacity
				<< ")";
		}
	};

	struct Point {
		Point() = default;
		Point(double x, double y)
			: x(x)
			, y(y) {
		}
		double x = 0;
		double y = 0;
	};

	struct RenderContext {
		RenderContext(std::ostream& out)
			: out(out) {
		}

		RenderContext(std::ostream& out, int indent_step, int indent = 0)
			: out(out)
			, indent_step(indent_step)
			, indent(indent) {
		}

		RenderContext Indented() const {
			return { out, indent_step, indent + indent_step };
		}

		void RenderIndent() const {
			for (int i = 0; i < indent; ++i) {
				out.put(' ');
			}
		}

		std::ostream& out;
		int indent_step = 0;
		int indent = 0;
	};

	class Object {
	public:
		void Render(const RenderContext& context) const;

		virtual ~Object() = default;

	private:
		virtual void RenderObject(const RenderContext& context) const = 0;
	};

	class ObjectContainer {
	public:
		template <typename T>
		void Add(T obj) {
			AddPtr(std::make_unique<T>(std::move(obj)));
		}

		virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

	protected:
		~ObjectContainer() = default;
	};

	class Drawable {
	public:
		virtual ~Drawable() = default;
		virtual void Draw(ObjectContainer& container) const = 0;
	};


	template <typename Owner>
	class PathProps {
	public:
		Owner& SetFillColor(Color color) {
			fill_color_ = std::move(color);
			return AsOwner();
		}
		Owner& SetStrokeColor(Color color) {
			stroke_color_ = std::move(color);
			return AsOwner();
		}
		Owner& SetStrokeWidth(double width) {
			width_ = std::move(width);
			return AsOwner();
		}
		Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
			line_cap_ = line_cap;
			return AsOwner();
		}
		Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
			line_join_ = line_join;
			return AsOwner();
		}
	protected:
		~PathProps() = default;

		void RenderAttrs(std::ostream& out) const {
			using namespace std::literals;

			if (fill_color_) {
				out << " fill=\\\""sv;
				std::visit(ColorPrinter{ out }, *fill_color_);
				out << "\\\""sv;
			}
			if (stroke_color_) {
				out << " stroke=\\\""sv;
				std::visit(ColorPrinter{ out }, *stroke_color_);
				out << "\\\""sv;
			}
			if (width_) {
				out << " stroke-width=\\\""sv << *width_ << "\\\""sv;
			}
			if (line_cap_) {
				out << " stroke-linecap=\\\""sv << *line_cap_ << "\\\""sv;
			}
			if (line_join_) {
				out << " stroke-linejoin=\\\""sv << *line_join_ << "\\\""sv;
			}
		}

	private:
		Owner& AsOwner() {
			return static_cast<Owner&>(*this);
		}

		std::optional<Color> fill_color_;
		std::optional<Color> stroke_color_;
		std::optional<double> width_;
		std::optional<StrokeLineCap> line_cap_;
		std::optional<StrokeLineJoin> line_join_;
	};


	class Circle final : public Object, public PathProps<Circle> {
	public:
		Circle& SetCenter(Point center);
		Circle& SetRadius(double radius);

	private:
		void RenderObject(const RenderContext& context) const override;

		Point center_;
		double radius_ = 1.0;
	};

	class Polyline final : public Object, public PathProps<Polyline> {
	public:
		Polyline& AddPoint(Point point);

	private:
		void RenderObject(const RenderContext& context) const override;

		std::vector<Point> polylines_;
	};

	class Text final : public Object, public PathProps<Text> {
	public:
		Text& SetPosition(Point pos);

		Text& SetOffset(Point offset);

		Text& SetFontSize(uint32_t size);

		Text& SetFontFamily(std::string font_family);

		Text& SetFontWeight(std::string font_weight);

		Text& SetData(std::string data);

	private:
		void RenderObject(const RenderContext& context) const override;

		Point position_ = { 0.0, 0.0 };
		Point offset_ = { 0.0, 0.0 };
		uint32_t fontSize_ = 1;
		std::string fontFamily_;
		std::string fontWeight_;
		std::string data_;
	};

	class Document : public ObjectContainer {
	public:
		void AddPtr(std::unique_ptr<Object>&& obj) override;

		void Render(std::ostream& out) const;

	private:
		std::vector<std::unique_ptr<Object>> objects_;
	};

}  // namespace svg