#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

namespace json {

	class Node;
	using Dict = std::map<std::string, Node>;
	using Array = std::vector<Node>;

	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class Node {
	public:
		using Value = std::variant<std::nullptr_t, std::string, int, double, bool, Array, Dict>;

		Node() = default;

		template<typename T>
		Node(T value) : value_(std::move(value)) { }

		bool IsInt() const;
		bool IsDouble() const;
		bool IsPureDouble() const;
		bool IsBool() const;
		bool IsString() const;
		bool IsNull() const;
		bool IsArray() const;
		bool IsMap() const;

		int AsInt() const;
		bool AsBool() const;
		double AsDouble() const;
		const std::string& AsString() const;
		const Array& AsArray() const;
		const Dict& AsMap() const;

		Value& GetValue();
		const Value& GetValue() const;

		bool operator==(const Node& rhs) const;
		bool operator!=(const Node& rhs) const;

	private:
		Value value_;
	};

	class Document {
	public:
		explicit Document(Node root);

		const Node& GetRoot() const;

		bool operator==(const Document& rhs) const;
		bool operator!=(const Document& rhs) const;

	private:
		Node root_;
	};

	Document Load(std::istream& input);

	struct PrintContext {
		std::ostream& out;
		int indent_step = 4;
		int indent = 0;

		void PrintIndent(int x = 0) const {
			for (int i = 0; i < indent+x; ++i) {
				out.put(' ');
			}
		}
		PrintContext Indented() const {
			return { out, indent_step, indent_step + indent };
		}
	};

	struct ValuePrinter {
		PrintContext ctx;

		void operator()(std::nullptr_t);
		void operator()(std::string value);
		void operator()(int value);
		void operator()(double value);
		void operator()(bool value);
		void operator()(Array array);
		void operator()(Dict dict);
	};

	void Print(const Document& doc, std::ostream& output);

}  // namespace json