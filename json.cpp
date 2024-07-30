#include "json.h"

using namespace std;

namespace json {

	namespace {

		using Number = std::variant<int, double>;

		json::Node LoadNode(istream& input);

		Node LoadNull(istream& input) {
			const string nameNull = "null";
			for (size_t i = 0; i < nameNull.size(); i++) {
				if (nameNull.at(i) == input.get()) continue;
				else throw json::ParsingError("Null parsing error");
			}
			if ((input.peek() >= 'A' && input.peek() <= 'Z') || (input.peek() >= 'a' && input.peek() <= 'z')) {
				throw ParsingError("Null parsing error");
			}
			return { nullptr };
		}

		std::string LoadString(std::istream& input) {
			using namespace std::literals;

			auto it = std::istreambuf_iterator<char>(input);
			auto end = std::istreambuf_iterator<char>();
			std::string s;
			while (true) {
				if (it == end) {
					// Поток закончился до того, как встретили закрывающую кавычку?
					throw ParsingError("String parsing error");
				}
				const char ch = *it;
				if (ch == '"') {
					// Встретили закрывающую кавычку
					++it;
					break;
				}
				else if (ch == '\\') {
					// Встретили начало escape-последовательности
					++it;
					if (it == end) {
						// Поток завершился сразу после символа обратной косой черты
						throw ParsingError("String parsing error");
					}
					const char escaped_char = *(it);
					// Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
					switch (escaped_char) {
					case 'n':
						s.push_back('\n');
						break;
					case 't':
						s.push_back('\t');
						break;
					case 'r':
						s.push_back('\r');
						break;
					case '"':
						s.push_back('"');
						break;
					case '\\':
						s.push_back('\\');
						break;
					default:
						// Встретили неизвестную escape-последовательность
						throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
					}
				}
				else if (ch == '\n' || ch == '\r') {
					// Строковый литерал внутри- JSON не может прерываться символами \r или \n
					throw ParsingError("Unexpected end of line"s);
				}
				else {
					// Просто считываем очередной символ и помещаем его в результирующую строку
					s.push_back(ch);
				}
				++it;
			}

			return s;
		}

		json::Node LoadNumber(std::istream& input) {
			using namespace std::literals;

			std::string parsed_num;

			// Считывает в parsed_num очередной символ из input
			auto read_char = [&parsed_num, &input] {
				parsed_num += static_cast<char>(input.get());
				if (!input) {
					throw ParsingError("Failed to read number from stream"s);
				}
				};

			// Считывает одну или более цифр в parsed_num из input
			auto read_digits = [&input, read_char] {
				if (!std::isdigit(input.peek())) {
					throw ParsingError("A digit is expected"s);
				}
				while (std::isdigit(input.peek())) {
					read_char();
				}
				};

			if (input.peek() == '-') {
				read_char();
			}
			// Парсим целую часть числа
			if (input.peek() == '0') {
				read_char();
				// После 0 в JSON не могут идти другие цифры
			}
			else {
				read_digits();
			}

			bool is_int = true;
			// Парсим дробную часть числа
			if (input.peek() == '.') {
				read_char();
				read_digits();
				is_int = false;
			}

			// Парсим экспоненциальную часть числа
			if (int ch = input.peek(); ch == 'e' || ch == 'E') {
				read_char();
				if (ch = input.peek(); ch == '+' || ch == '-') {
					read_char();
				}
				read_digits();
				is_int = false;
			}

			try {
				if (is_int) {
					// Сначала пробуем преобразовать строку в int
					try {
						return std::stoi(parsed_num);
					}
					catch (...) {
						// В случае неудачи, например, при переполнении,
						// код ниже попробует преобразовать строку в double
					}
				}
				return std::stod(parsed_num);
			}
			catch (...) {
				throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
			}
		}

		json::Node LoadBool(istream& input) {
			const string nameFalse = "false";
			const string nameTrue = "true";
			char c = input.get();
			bool value = (c == 't');
			std::string const* name = value ? &nameTrue : &nameFalse;
			for (size_t i = 1; i < name->size(); i++) {
				if (name->at(i) == input.get()) continue;
				else throw ParsingError("Bool parsing error");
			}
			if ((input.peek() >= 'A' && input.peek() <= 'Z') || (input.peek() >= 'a' && input.peek() <= 'z')) {
				throw ParsingError("Bool parsing error");
			}
			return Node(value);
		}

		json::Node LoadArray(istream& input) {
			Array result;
			if (input.peek() == -1) throw ParsingError("Array parsing error");

			for (char c; input >> c && c != ']';) {
				if (c != ',') {
					input.putback(c);
				}
				result.push_back(LoadNode(input));
			}

			return Node(std::move(result));
		}

		json::Node LoadDict(istream& input) {
			Dict result;
			if (input.peek() == -1) throw ParsingError("Array parsing error");

			for (char c; input >> c && c != '}';) {
				if (c == ',') {
					input >> c;
				}

				string key = LoadString(input);
				input >> c;
				result.insert({ std::move(key), LoadNode(input) });
			}

			return Node(std::move(result));
		}

		json::Node LoadNode(istream& input) {
			char c;
			input >> c;

			if (c == 'n') {
				input.putback(c);
				return LoadNull(input);
			}
			else if (c == '"') {
				return LoadString(input);
			}
			else if (c == 't' || c == 'f') {
				input.putback(c);
				return LoadBool(input);
			}
			else if (c == '[') {
				return LoadArray(input);
			}
			else if (c == '{') {
				return LoadDict(input);
			}
			else {
				input.putback(c);
				return LoadNumber(input);
			}
		}

	} // namespace

	// ------- Node::Node -------- 

	//json::Node::Node(std::nullptr_t)
	//	: value_(nullptr) { }

	//json::Node::Node(std::string value)
	//	: value_(std::move(value)) { }

	//json::Node::Node(int value)
	//	: value_(value) { }

	//json::Node::Node(double value)
	//	: value_(value) { }

	//json::Node::Node(bool value)
	//	: value_(value) { }

	//json::Node::Node(Array array)
	//	: value_(std::move(array)) { }

	//json::Node::Node(Dict map)
	//	: value_(std::move(map)) { }

	// ------- Is -------- 

	bool json::Node::IsInt() const {
		return holds_alternative<int>(value_);
	}

	bool json::Node::IsDouble() const {
		return holds_alternative<double>(value_) || holds_alternative<int>(value_);
	}

	bool json::Node::IsPureDouble() const {
		return holds_alternative<double>(value_);
	}

	bool json::Node::IsBool() const {
		return holds_alternative<bool>(value_);
	}

	bool json::Node::IsString() const {
		return holds_alternative<std::string>(value_);
	}

	bool json::Node::IsNull() const {
		return holds_alternative<std::nullptr_t>(value_);
	}

	bool json::Node::IsArray() const {
		return holds_alternative<Array>(value_);
	}

	bool json::Node::IsMap() const {
		return holds_alternative<Dict>(value_);
	}

	// ------- As -------- 

	int json::Node::AsInt() const {
		if (!IsInt()) throw std::logic_error("wrong type");
		return std::get<int>(value_);
	}

	bool json::Node::AsBool() const {
		if (!IsBool()) throw std::logic_error("wrong type");
		return std::get<bool>(value_);
	}

	double json::Node::AsDouble() const {
		if (!IsDouble()) throw std::logic_error("wrong type");
		if (IsInt()) return static_cast<double>(std::get<int>(value_));
		else return std::get<double>(value_);
	}

	const std::string& json::Node::AsString() const {
		if (!IsString()) throw std::logic_error("wrong type");
		return std::get<std::string>(value_);
	}

	const json::Array& json::Node::AsArray() const {
		if (!IsArray()) throw std::logic_error("wrong type");
		return std::get<Array>(value_);
	}

	const json::Dict& json::Node::AsMap() const {
		if (!IsMap()) throw std::logic_error("wrong type");
		return std::get<Dict>(value_);
	}

	// ------- Other --------

	void Print(const Document& doc, std::ostream& out) {
		ValuePrinter printer{ {out,4,0} };
		std::visit(printer, doc.GetRoot().GetValue());
	}

	const json::Node::Value& json::Node::GetValue() const {
		return value_;
	}

	json::Node::Value& json::Node::GetValue() {
		return value_;
	}

	bool json::Node::operator==(const Node& rhs) const {
		return value_ == rhs.value_;
	}

	bool json::Node::operator!=(const Node& rhs) const {
		return !(value_ == rhs.value_);
	}

	// ------- Document --------

	json::Document::Document(Node root)
		: root_(std::move(root)) { }

	const Node& json::Document::GetRoot() const {
		return root_;
	}

	bool json::Document::operator==(const Document& rhs) const {
		return  root_ == rhs.root_;
	}

	bool json::Document::operator!=(const Document& rhs) const {
		return !(root_ == rhs.root_);
	}

	json::Document Load(istream& input) {
		return json::Document{ json::LoadNode(input) };
	}

	// ------- ValuePrinter --------

	void ValuePrinter::operator()(std::nullptr_t) {
		ctx.out << "null";
	}

	void ValuePrinter::operator()(std::string value) {
		ctx.out << "\"" << value << "\"";
	}

	void ValuePrinter::operator()(int value) {
		ctx.out << value;
	}

	void ValuePrinter::operator()(double value) {
		ctx.out << value;
	}

	void ValuePrinter::operator()(bool value) {
		ctx.out << std::boolalpha << value;
	}

	void ValuePrinter::operator()(Array array) {
		ctx.out << "[" << std::endl;
		bool first = true;
		for (const auto& elem : array) {
			if (!first) ctx.out << "," << std::endl;
			else first = false;
			ctx.PrintIndent();
			ctx.PrintIndent(-4);
			std::visit(ValuePrinter{ ctx.Indented() }, elem.GetValue());


		}
		ctx.out << std::endl;
		ctx.PrintIndent();
		ctx.out << "]";
	}

	void ValuePrinter::operator()(Dict dict) {
		ctx.PrintIndent();
		ctx.out << "{" << std::endl;
		bool first = true;
		for (const auto& [key, value] : dict) {
			if (!first) ctx.out << "," << std::endl;
			else first = false;
			ctx.Indented().PrintIndent();
			ctx.out << "\"" << key << "\": ";
			std::visit(ValuePrinter{ ctx.Indented() }, value.GetValue());
		}
		ctx.out << std::endl;
		ctx.PrintIndent();
		ctx.out << "}";
	}

}  // namespace json