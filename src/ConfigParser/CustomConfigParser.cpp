#include "CustomConfigParser.h"

#include <iostream>
#include <sstream>

std::vector<ConfigSection> CustomConfigParser::parse(const std::string& content) const
{
    std::vector<ConfigSection> configSections;
    std::istringstream iss(content);
    std::string currentSection;
    std::string line;
    bool inMultilineComment = false;
    while (std::getline(iss, line))
    {
        if (inMultilineComment)
        {
            auto endCommentPos = line.find("*/");
            if (endCommentPos != std::string::npos)
            {
                inMultilineComment = false;
                line.erase(0, endCommentPos + 2);
            }
            else
            {
                continue;
            }
        }
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        if (line.empty()) continue;

        auto startCommentPos = line.find("/*");
        if (startCommentPos != std::string::npos)
        {
            auto endCommentPos = line.find("*/", startCommentPos);
            if (endCommentPos != std::string::npos)
            {
                line.erase(startCommentPos, endCommentPos - startCommentPos + 2);
            }
            else
            {
                line.erase(startCommentPos);
                inMultilineComment = true;
            }
        }
        if (line.empty()) continue;

        if (line[0] == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
            configSections.emplace_back(ConfigSection{ currentSection, {} });
        }
        else
        {
            configSections.back().lines.push_back(std::move(line));
        }
    }
    return configSections;
}
