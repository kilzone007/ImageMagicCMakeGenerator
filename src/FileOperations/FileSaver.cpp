#include "FileSaver.h"

#include <fstream>
#include <stdexcept>

FileSaver::FileSaver(const std::filesystem::path& filename)
    : m_filename(filename)
{
}

void FileSaver::saveToFile(const std::string& text)
{
    if (m_filename.empty())
    {
        throw std::invalid_argument("Filename cannot be empty.");
    }

    std::ofstream file(m_filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file.");
    }

    file << text;
    file.close();
}