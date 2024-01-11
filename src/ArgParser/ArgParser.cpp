#include "ArgParser.h"

ArgParser::ArgParser(int argc, char* argv[]) : m_args(argc - 1)
{
    for (int i = 1; i < argc; ++i)
    {
        m_args[i - 1] = argv[i];
    }
}

bool ArgParser::hasOption(const std::string& option) const
{
    return std::find(m_args.begin(), m_args.end(), option) != m_args.end();
}

std::string ArgParser::getOptionValue(const std::string& option) const
{
    auto it = std::find(m_args.begin(), m_args.end(), option);
    if (it != m_args.end() && ++it != m_args.end())
    {
        return *it;
    }
    return {};
}
