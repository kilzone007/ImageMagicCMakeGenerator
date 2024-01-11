#pragma once

#include "IConfigParser.h"

// Конкретная реализация парсера конфигурации
class CustomConfigParser : public IConfigParser
{
public:
    std::vector<ConfigSection> parse(const std::string& filename) const override;
};