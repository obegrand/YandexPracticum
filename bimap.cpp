#include "bimap.h"
#include <unordered_map>

struct BiMap::Impl {
	bool Add(std::string_view key, std::string_view value) {
		if (key_vocabulary.contains(key) || key_vocabulary.contains(value)) {
			return false;
		}
		key_vocabulary[key] = value;
		value_vocabulary[value] = key;
		return true;
	}

	std::optional<std::string_view> FindValue(std::string_view key) {
		if (key_vocabulary.contains(key))
			return key_vocabulary.at(key);
		else
			return std::nullopt;
	}

	std::optional<std::string_view> FindKey(std::string_view value) {
		if (value_vocabulary.contains(value))
			return value_vocabulary.at(value);
		else
			return std::nullopt;
	}
private:
	std::unordered_map<std::string_view, std::string_view> key_vocabulary;
	std::unordered_map<std::string_view, std::string_view> value_vocabulary;
};

BiMap::BiMap() : impl_(std::make_unique<Impl>()) {}

BiMap::~BiMap() = default;

BiMap::BiMap(const BiMap& other) : impl_(new Impl(*other.impl_)) {}

BiMap::BiMap(BiMap&& other) noexcept : impl_(std::move(other.impl_)) {}


BiMap& BiMap::operator=(const BiMap& other) {
	if (this != &other) {
		impl_ = std::make_unique<Impl>(*other.impl_);
	}
	return *this;
}

BiMap& BiMap::operator=(BiMap&& other) noexcept {
	if (this != &other) {
		impl_ = std::move(other.impl_);
	}
	return *this;
}

bool BiMap::Add(std::string_view key, std::string_view value) {
	return impl_->Add(key, value);
}

std::optional<std::string_view> BiMap::FindValue(std::string_view key) const noexcept {
	return impl_->FindValue(key);
}

std::optional<std::string_view> BiMap::FindKey(std::string_view value) const noexcept {
	return impl_->FindKey(value);
}