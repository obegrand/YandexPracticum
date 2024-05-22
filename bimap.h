#pragma once

#include <optional>
#include <string_view>
#include <memory>

class BiMap {
public:
    BiMap();
    ~BiMap(); // Explicit destructor to handle unique_ptr correctly

    BiMap(const BiMap& other); // Copy constructor
    BiMap(BiMap&& other) noexcept; // Move constructor
    BiMap& operator=(const BiMap& other); // Copy assignment
    BiMap& operator=(BiMap&& other) noexcept; // Move assignment

    bool Add(std::string_view key, std::string_view value);

    std::optional<std::string_view> FindValue(std::string_view key) const noexcept;

    std::optional<std::string_view> FindKey(std::string_view value) const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};