#include "CustomConfigParser.h"

#include <iostream>
#include <fstream>

std::vector<ConfigSection> CustomConfigParser::parse(const std::string& filename) const
{
    std::vector<ConfigSection> configSections;
    std::ifstream file(filename);
    std::string currentSection;

    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return configSections;
    }

    std::string line;
    bool inMultilineComment = false;

    while (std::getline(file, line))
    {
        if (inMultilineComment)
        {
            auto endCommentPos = line.find("*/");
            if (endCommentPos != std::string::npos)
            {
                inMultilineComment = false;
                line.erase(0, endCommentPos + 2); // Удаляем закрытие комментария
            }
            else
            {
                continue; // Пропускаем строки внутри комментария
            }
        }

        line.erase(0, line.find_first_not_of(" \t\r\n"));  // Удаляем начальные пробелы и табуляции

        if (line.empty()) continue;  // Пропускаем пустые строки

        // Обработка многострочных комментариев
        auto startCommentPos = line.find("/*");
        if (startCommentPos != std::string::npos)
        {
            auto endCommentPos = line.find("*/", startCommentPos);
            if (endCommentPos != std::string::npos)
            {
                line.erase(startCommentPos, endCommentPos - startCommentPos + 2); // Удаляем комментарий
            }
            else
            {
                line.erase(startCommentPos); // Удаляем начало комментария
                inMultilineComment = true;
            }
        }

        if (line.empty()) continue;  // Проверка после удаления комментариев

        if (line[0] == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
            configSections.push_back({ currentSection, {} });
        }
        else
        {
            configSections.back().lines.push_back(line);
        }
    }

    file.close();
    return configSections;
}
