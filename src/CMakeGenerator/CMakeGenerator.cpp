#include "CMakeGenerator.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

const std::string CMAKE_MIN_VERSION = "3.10";
const std::string PROJECT_NAME_VARIABLE = "${PROJECT_NAME}";

CMakeGenerator::CMakeGenerator(std::unique_ptr<IConfigParser> p)
    : m_parser(std::move(p))
{
}

void CMakeGenerator::generate(const std::string& configFilename)
{
    auto sections = m_parser->parse(configFilename);
    std::filesystem::path filePath(configFilename);
    std::string parentDirectoryName = filePath.parent_path().filename().string();

    std::ofstream cmakeFile(filePath.replace_filename("CMakeLists.txt"));
    if (!cmakeFile.is_open())
    {
        throw std::runtime_error("Failed to open CMakeLists.txt for writing!");
    }

    writeCMakeHeader(cmakeFile, parentDirectoryName);
    processSections(cmakeFile, sections);

    cmakeFile.close();

    std::cout << "CMakeLists.txt has been generated successfully!" << std::endl;
}

void CMakeGenerator::writeCMakeHeader(std::ofstream& file, const std::string& projectName)
{
    if (!(file << "cmake_minimum_required(VERSION " << CMAKE_MIN_VERSION << ")\n"
        << "project(" << projectName << ")\n\n"))
    {
        throw std::runtime_error("Failed to write CMake header to the file!");
    }
}

void CMakeGenerator::processSections(std::ofstream& file, const std::vector<ConfigSection>& sections)
{
    for (const auto& section : sections)
    {
        if (section.name == "DIRECTORIES")
        {
            processDirectoriesSection(file, section);
            break;
        }
        else if (section.name == "EXCLUDES")
        {
            processExcludesSection(file, section);
        }
    }

    // После обработки нужных секций, вызовите функции для "APP", "STATIC", "DLL"
    for (const auto& section : sections)
    {
        if (section.name == "APP")
        {
            processAppSection(file);
            break;
        }
        else if (section.name == "STATIC")
        {
            processStaticSection(file);
            break;
        }
        else if (section.name == "DLL")
        {
            processDllSection(file);
            break;
        }
    }

    // Обработка оставшихся секций
    for (const auto& section : sections)
    {
        if (section.name == "INCLUDES")
        {
            processIncludesSection(file, section);
        }
        else if (section.name == "DEPENDENCIES")
        {
            processDependenciesSection(file, section);
        }
        else if (section.name == "DEFINES")
        {
            processDefinesSection(file, section);
        }
    }
}

void CMakeGenerator::processDirectoriesSection(std::ofstream& file, const ConfigSection& section)
{
    file << "set(" << PROJECT_NAME_VARIABLE << "_SOURCES)\n\n";
    for (const auto& line : section.lines)
    {
        file << "file(GLOB _tmp_files\n"
            << "${CMAKE_CURRENT_LIST_DIR}\\" << line << "\\*.c\n"
            << "${CMAKE_CURRENT_LIST_DIR}\\" << line << "\\*.h\n"
            << "${CMAKE_CURRENT_LIST_DIR}\\" << line << "\\*.cpp\n"
            << "${CMAKE_CURRENT_LIST_DIR}\\" << line << "\\*.hpp"
            << ")\n";

        file << "list(APPEND " << PROJECT_NAME_VARIABLE << "_SOURCES ${_tmp_files})\n\n";
    }
    file << "unset(_tmp_files)\n\n";
}

void CMakeGenerator::processExcludesSection(std::ofstream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "list(FILTER " << PROJECT_NAME_VARIABLE << "_SOURCES EXCLUDE REGEX *" << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processIncludesSection(std::ofstream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_include_directories(" << PROJECT_NAME_VARIABLE << " PUBLIC " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processDependenciesSection(std::ofstream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_link_libraries(" << PROJECT_NAME_VARIABLE << " " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processDefinesSection(std::ofstream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_compile_definitions(" << PROJECT_NAME_VARIABLE << " PRIVATE " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processAppSection(std::ofstream& file)
{
    file << "add_executable(" << PROJECT_NAME_VARIABLE << " ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}

void CMakeGenerator::processStaticSection(std::ofstream& file)
{
    file << "add_library(" << PROJECT_NAME_VARIABLE << " STATIC ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}

void CMakeGenerator::processDllSection(std::ofstream& file)
{
    file << "add_library(" << PROJECT_NAME_VARIABLE << " SHARED ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}