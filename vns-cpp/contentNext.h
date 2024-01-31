#ifndef CONTENT_NEXT_H
#define CONTENT_NEXT_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>

#include "libs/nlohmann/json.hpp"

using MultiTargetsType = std::vector<std::unordered_map<std::string, std::string> >;

using ContentNextValueType = std::variant<std::string, MultiTargetsType>;

class ContentNext
{
public:
    ContentNext(std::string type, ContentNextValueType target) : type_(std::move(type)), target_(std::move(target))
    {}

    explicit ContentNext(const std::unordered_map<std::string, ContentNextValueType> &data) : ContentNext(
            std::get<std::string>(data.contains("type") ? data.at("type") : "default"),
            data.contains("target") ? data.at("target") : "")
    {}

    ContentNext() : ContentNext("default", "")
    {}

    [[nodiscard]] std::string get_type() const;

    [[nodiscard]] std::string get_target() const;

    [[nodiscard]] MultiTargetsType get_targets() const;

    [[nodiscard]] bool has_single_target() const;

    [[nodiscard]] bool has_multi_targets() const;

    [[nodiscard]] bool is_null() const;

    [[nodiscard]] std::unordered_map<std::string, ContentNextValueType> to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

private:
    std::string type_;
    ContentNextValueType target_;
};

const ContentNext kNullContentNext;
#endif
