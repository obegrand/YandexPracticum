#pragma once

#include "json.h"
#include <optional>
#include <vector>

namespace json {

	class JsonDictKey;
	class JsonDictItem;
	class JsonArrayItem;

	class Builder {
	public:
		Builder();
		JsonDictKey Key(std::string key);
		Builder& Value(Node::Value value);
		JsonDictItem StartDict();
		JsonArrayItem StartArray();
		Builder& EndDict();
		Builder& EndArray();
		Node& Build();
	private:
		template<typename Type>
		void AddNode(Type type);

		Node node_{ nullptr };
		std::vector<Node*> node_stack_;
		std::optional<std::string> key_{ std::nullopt };
	};

	class JsonDictKey {
	public:
		JsonDictKey(Builder& builder) : builder_(builder) { }

		JsonDictItem Value(Node::Value value);
		JsonDictItem StartDict();
		JsonArrayItem StartArray();
	private:
		Builder& builder_;
	};

	class JsonDictItem {
	public:
		JsonDictItem(Builder& builder) : builder_(builder) { }

		JsonDictKey Key(std::string key);
		Builder& EndDict();
	private:
		Builder& builder_;
	};

	class JsonArrayItem {
	public:
		JsonArrayItem(Builder& builder) : builder_(builder) { }
		
		JsonArrayItem Value(Node::Value value);
		JsonDictItem StartDict();
		JsonArrayItem StartArray();
		Builder& EndArray();
	private:
		Builder& builder_;
	};

	template<typename Type>
	void Builder::AddNode(Type type) {
		Node* node_stack = node_stack_.back();
		if (node_stack->IsMap()) {
			if (!key_.has_value()) throw std::logic_error("key is empty");
			Dict& dict = std::get<Dict>(node_stack->GetValue());
			dict[key_.value()].GetValue() = std::move(type);
			node_stack_.push_back(&dict[std::move(key_.value())]);
			key_.reset();
		}
		else if (node_stack->IsArray()) {
			Array& arr = std::get<Array>(node_stack->GetValue());
			arr.emplace_back().GetValue() = std::move(type);
			node_stack_.push_back(&arr.back());

		}
		else if (node_stack->IsNull()) {
			node_.GetValue() = std::move(type);
		}
		else {
			throw std::logic_error("overloading the value");
		}
	}

} // namespace json