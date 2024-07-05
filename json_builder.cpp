#include "json_builder.h"

namespace json {
	
	//---------- Builder ------------

	Builder::Builder() {
		node_stack_.push_back(&node_);
	}

	JsonDictKey Builder::Key(std::string key) {
		Node* node_stack = node_stack_.back();
		if ((!node_stack->IsMap() || !node_stack->IsArray()) && key_.has_value())
			throw std::logic_error("reuse key or outside array or dict");
		key_ = std::move(key);
		return *this;
	}

	Builder& Builder::Value(Node::Value value) {
		Node* node_stack = node_stack_.back();
		if (node_stack->IsMap()) {
			if (!key_.has_value()) throw std::logic_error("key is empty");
			std::get<Dict>(node_stack->GetValue())[key_.value()].GetValue() = std::move(value);
			key_.reset();
		}
		else if (node_stack->IsArray()) {
			std::get<Array>(node_stack->GetValue()).emplace_back().GetValue() = std::move(value);
		}
		else if (node_stack->IsNull()) {
			node_.GetValue() = std::move(value);
		}
		else {
			throw std::logic_error("overloading the value");
		}
		return *this;
	}

	JsonDictItem Builder::StartDict() {
		AddNode(std::move(Dict{}));
		return *this;
	}

	JsonArrayItem Builder::StartArray() {
		AddNode(std::move(Array{}));
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

	//---------- JsonDictKey ------------

	JsonDictItem JsonDictKey::Value(Node::Value value) {
		return JsonDictItem(builder_.Value(value));
	}

	JsonDictItem JsonDictKey::StartDict() {
		return builder_.StartDict();
	}

	JsonArrayItem JsonDictKey::StartArray() {
		return builder_.StartArray();
	}

	//---------- JsonDictItem ------------

	JsonDictKey JsonDictItem::Key(std::string key) {
		return builder_.Key(key);
	}

	Builder& JsonDictItem::EndDict() {
		return builder_.EndDict();
	}

	//---------- JsonArrayItem ------------

	JsonArrayItem JsonArrayItem::Value(Node::Value value) {
		return JsonArrayItem(builder_.Value(value));
	}

	JsonDictItem JsonArrayItem::StartDict() {
		return builder_.StartDict();
	}

	JsonArrayItem JsonArrayItem::StartArray() {
		return builder_.StartArray();
	}

	Builder& JsonArrayItem::EndArray() {
		return builder_.EndArray();
	}

} // namespace json