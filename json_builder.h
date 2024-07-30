#pragma once

#include "json.h"
#include <optional>
#include <vector>

namespace json {

	class Builder {
	private:
		class JsonDictKey;
		class JsonDictItem;
		class JsonArrayItem;
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
		void AddNode(Node node, bool shouldPushOnStack = false);

		Node node_{ nullptr };
		std::vector<Node*> node_stack_;
		std::optional<std::string> key_{ std::nullopt };
	};

	class Builder::JsonDictKey {
	public:
		JsonDictKey(Builder& builder) : builder_(builder) { }

		JsonDictItem Value(Node::Value value);
		JsonDictItem StartDict();
		JsonArrayItem StartArray();
	private:
		Builder& builder_;
	};

	class Builder::JsonDictItem {
	public:
		JsonDictItem(Builder& builder) : builder_(builder) { }

		JsonDictKey Key(std::string key);
		Builder& EndDict();
	private:
		Builder& builder_;
	};

	class Builder::JsonArrayItem {
	public:
		JsonArrayItem(Builder& builder) : builder_(builder) { }

		JsonArrayItem Value(Node::Value value);
		JsonDictItem StartDict();
		JsonArrayItem StartArray();
		Builder& EndArray();
	private:
		Builder& builder_;
	};

} // namespace json