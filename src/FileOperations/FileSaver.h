#pragma once

#include <string>
#include <filesystem>

class FileSaver
{
public:
    FileSaver(const std::filesystem::path& filename);

    void saveToFile(const std::string& text);

private:
    std::filesystem::path m_filename;
};