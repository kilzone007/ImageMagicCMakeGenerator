#pragma once

#include <vector>
#include <string>

struct ConfigSection
{
    std::string name;
    std::vector<std::string> lines;
};

class IConfigParser
{
public:
    virtual std::vector<ConfigSection> parse(const std::string& content) const = 0;

    virtual ~IConfigParser() = default;
};