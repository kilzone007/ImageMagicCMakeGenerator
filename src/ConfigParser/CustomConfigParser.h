#pragma once

#include "IConfigParser.h"

class CustomConfigParser : public IConfigParser
{
public:
    std::vector<ConfigSection> parse(const std::string& filename) const override;
};