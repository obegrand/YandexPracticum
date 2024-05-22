#include "bimap.h"
#include <unordered_map>

struct BiMap::Impl {
    Impl() = default;

    Impl(const Impl& other)
        : key_vocabulary(other.key_vocabulary), value_vocabulary(other.value_vocabulary) {}

    bool Add(std::string key, std::string value) {
        if (key_vocabulary.find(key) != key_vocabulary.end() || value_vocabulary.find(value) != value_vocabulary.end()) {
            return false;
        }
        key_vocabulary[key] = value;
        value_vocabulary[value] = key;
        return true;
    }

    std::optional<std::string_view> FindValue(std::string_view key) {
        auto it = key_vocabulary.find(std::string(key));
        if (it != key_vocabulary.end())
            return it->second;
        else
            return std::nullopt;
    }

    std::optional<std::string_view> FindKey(std::string_view value) {
        auto it = value_vocabulary.find(std::string(value));
        if (it != value_vocabulary.end())
            return it->second;
        else
            return std::nullopt;
    }
private:
    std::unordered_map<std::string, std::string> key_vocabulary;
    std::unordered_map<std::string, std::string> value_vocabulary;
};

BiMap::BiMap() : impl_(std::make_unique<Impl>()) {}

BiMap::~BiMap() = default;

BiMap::BiMap(const BiMap& other) : impl_(new Impl(*(other.impl_))) {}

BiMap& BiMap::operator=(const BiMap& other) {
    if (this != &other) {
        // Deep copy the implementation object
        impl_ = std::make_unique<Impl>(*other.impl_);
    }
    return *this;
}

bool BiMap::Add(std::string_view key, std::string_view value) {
    return impl_->Add(std::string(key), std::string(value));
}

std::optional<std::string_view> BiMap::FindValue(std::string_view key) const noexcept {
	return impl_->FindValue(key);
}

std::optional<std::string_view> BiMap::FindKey(std::string_view value) const noexcept {
	return impl_->FindKey(value);
}