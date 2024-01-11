#include "FileReader.h"

FileReader::FileReader(const std::filesystem::path& filename)
    : m_filename(filename)
{
}


std::string FileReader::readFile()
{
    if (m_filename.empty())
    {
        throw std::invalid_argument("Filename cannot be empty.");
    }

    std::ifstream file(m_filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file.");
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return text;
}