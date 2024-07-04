#include "json_builder.h"

namespace json {
	Builder::Builder() {
		node_stack_.push_back(&node_);
	}

	Builder& Builder::Key(std::string key) {
		Node* node_stack = node_stack_.back();
		if ((!node_stack->IsDict() || !node_stack->IsArray()) && key_.has_value())
			throw std::logic_error("reuse key or outside array or dict");
		key_ = std::move(key);
		return *this;
	}

	Builder& Builder::Value(Node::Value value) {
		Node* node_stack = node_stack_.back();
		if (node_stack->IsDict()) {
			if (!key_.has_value()) throw std::logic_error("key is empty");
			Dict& dict = std::get<Dict>(node_stack->GetValue());
			dict[key_.value()].GetValue() = std::move(value);
			key_.reset();
		}
		else if (node_stack->IsArray()) {
			Array& arr = std::get<Array>(node_stack->GetValue());
			arr.emplace_back().GetValue() = std::move(value);
		}
		else if (node_stack->IsNull()) {
			node_.GetValue() = std::move(value);
		}
		else {
			throw std::logic_error("overloading the value");
		}
		return *this;
	}

	Builder& Builder::StartDict() {
		AddNode(std::move(Dict{}));
		return *this;
	}

	Builder& Builder::StartArray() {
		AddNode(std::move(Array{}));
		return *this;
	}

	Builder& Builder::EndDict() {
		if (!node_stack_.back()->IsDict()) throw std::logic_error("node at back not an Dict");
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

} // namespace json