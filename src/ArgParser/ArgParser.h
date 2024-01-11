#pragma once

#include <string>
#include <vector>

class ArgParser
{
public:
    ArgParser(int argc, char* argv[]);

    bool hasOption(const std::string& option) const;
    std::string getOptionValue(const std::string& option) const;

private:
    std::vector<std::string> m_args;
};
