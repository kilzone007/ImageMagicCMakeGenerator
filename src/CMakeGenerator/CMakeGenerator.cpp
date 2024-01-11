#include "CMakeGenerator.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <stdexcept>

const std::string CMAKE_MIN_VERSION = "3.10";
const std::string PROJECT_NAME_VARIABLE = "${PROJECT_NAME}";

CMakeGenerator::CMakeGenerator(std::unique_ptr<IConfigParser> p)
    : m_parser(std::move(p))
{
}

std::string CMakeGenerator::generate(const std::string& content, const std::string& projectName)
{
    auto sections = m_parser->parse(content);
    
    std::ostringstream cmakeFile;

    writeCMakeHeader(cmakeFile, projectName);
    processSections(cmakeFile, sections);


    return cmakeFile.str();
}

std::string CMakeGenerator::getOutputFilename() const
{
    return "CMakeLists.txt";
}

void CMakeGenerator::writeCMakeHeader(std::ostream& file, const std::string& projectName)
{
    if (!(file << "cmake_minimum_required(VERSION " << CMAKE_MIN_VERSION << ")\n"
        << "project(" << projectName << ")\n\n"))
    {
        throw std::runtime_error("Failed to write CMake header to the file!");
    }
}

void CMakeGenerator::processSections(std::ostream& file, const std::vector<ConfigSection>& sections)
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

void CMakeGenerator::processDirectoriesSection(std::ostream& file, const ConfigSection& section)
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

void CMakeGenerator::processExcludesSection(std::ostream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "list(FILTER " << PROJECT_NAME_VARIABLE << "_SOURCES EXCLUDE REGEX *" << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processIncludesSection(std::ostream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_include_directories(" << PROJECT_NAME_VARIABLE << " PUBLIC " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processDependenciesSection(std::ostream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_link_libraries(" << PROJECT_NAME_VARIABLE << " " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processDefinesSection(std::ostream& file, const ConfigSection& section)
{
    for (const auto& line : section.lines)
    {
        file << "target_compile_definitions(" << PROJECT_NAME_VARIABLE << " PRIVATE " << line << ")\n";
    }
    file << "\n";
}

void CMakeGenerator::processAppSection(std::ostream& file)
{
    file << "add_executable(" << PROJECT_NAME_VARIABLE << " ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}

void CMakeGenerator::processStaticSection(std::ostream& file)
{
    file << "add_library(" << PROJECT_NAME_VARIABLE << " STATIC ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}

void CMakeGenerator::processDllSection(std::ostream& file)
{
    file << "add_library(" << PROJECT_NAME_VARIABLE << " SHARED ";
    file << "${" << PROJECT_NAME_VARIABLE << "_SOURCES}";
    file << ")\n\n";
}