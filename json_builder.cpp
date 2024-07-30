#include "json_builder.h"

namespace json {

	//---------- Builder ------------

	Builder::Builder() {
		node_stack_.push_back(&node_);
	}

	Builder::JsonDictKey Builder::Key(std::string key) {
		Node* node_stack = node_stack_.back();
		if ((!node_stack->IsMap() || !node_stack->IsArray()) && key_.has_value())
			throw std::logic_error("reuse key or outside array or dict");
		key_ = std::move(key);
		return *this;
	}

	Builder& Builder::Value(Node::Value value) {
		AddNode(std::move(value));
		return *this;
	}

	Builder::JsonDictItem Builder::StartDict() {
		AddNode(Dict{}, true);
		return *this;
	}

	Builder::JsonArrayItem Builder::StartArray() {
		AddNode(Array{}, true);
		return *this;
	}

	Builder& Builder::EndDict() {
		if (!node_stack_.back()->IsMap()) throw std::logic_error("node at back not an Dict");
		node_stack_.pop_back();
		return *this;
	}

	Builder& Builder::EndArray() {
		if (!node_stack_.back()->IsArray()) throw std::logic_error("node at back not an Array");
		node_stack_.pop_back();
		return *this;
	}

	Node& Builder::Build() {
		if (node_ == nullptr ||
			node_stack_.size() > 1 ||
			key_.has_value())
			throw std::logic_error("json document is not completed");
		return node_;
	}

	void Builder::AddNode(Node node, bool shouldPushOnStack) {
		Node* node_stack = node_stack_.back();
		if (node_stack->IsMap()) {
			if (!key_.has_value()) throw std::logic_error("key is empty");
			Dict& dict = std::get<Dict>(node_stack->GetValue());
			dict[std::move(key_.value())] = std::move(node);
			if (shouldPushOnStack) node_stack_.push_back(&dict[std::move(key_.value())]);
			key_.reset();
		}
		else if (node_stack->IsArray()) {
			Array& arr = std::get<Array>(node_stack->GetValue());
			arr.emplace_back() = std::move(node);
			if (shouldPushOnStack) node_stack_.push_back(&arr.back());

		}
		else if (node_stack->IsNull()) {
			node_ = std::move(node);
		}
		else {
			throw std::logic_error("overloading the value");
		}
	}

	//---------- JsonDictKey ------------

	Builder::JsonDictItem Builder::JsonDictKey::Value(Node::Value value) {
		return JsonDictItem(builder_.Value(std::move(value)));
	}

	Builder::JsonDictItem Builder::JsonDictKey::StartDict() {
		return builder_.StartDict();
	}

	Builder::JsonArrayItem Builder::JsonDictKey::StartArray() {
		return builder_.StartArray();
	}

	//---------- JsonDictItem ------------

	Builder::JsonDictKey Builder::JsonDictItem::Key(std::string key) {
		return builder_.Key(std::move(key));
	}

	Builder& Builder::JsonDictItem::EndDict() {
		return builder_.EndDict();
	}

	//---------- JsonArrayItem ------------

	Builder::JsonArrayItem Builder::JsonArrayItem::Value(Node::Value value) {
		return JsonArrayItem(builder_.Value(std::move(value)));
	}

	Builder::JsonDictItem Builder::JsonArrayItem::StartDict() {
		return builder_.StartDict();
	}

	Builder::JsonArrayItem Builder::JsonArrayItem::StartArray() {
		return builder_.StartArray();
	}

	Builder& Builder::JsonArrayItem::EndArray() {
		return builder_.EndArray();
	}

} // namespace json