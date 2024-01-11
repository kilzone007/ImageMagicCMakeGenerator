#pragma once

#include <vector>
#include <string>

// Структура для представления секции и связанных с ней строк
struct ConfigSection
{
    std::string name;
    std::vector<std::string> lines;
};

// Интерфейс для парсера конфигурации
class IConfigParser
{
public:
    virtual std::vector<ConfigSection> parse(const std::string& filename) const = 0;
    virtual ~IConfigParser() = default;
};