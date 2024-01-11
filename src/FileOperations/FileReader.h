#pragma once

#include <fstream>
#include <string>
#include <stdexcept>
#include <filesystem>

class FileReader
{
public:
    FileReader(const std::filesystem::path& filename);

    std::string readFile();

private:
    std::filesystem::path m_filename;
};