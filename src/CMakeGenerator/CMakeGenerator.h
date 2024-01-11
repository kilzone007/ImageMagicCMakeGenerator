#pragma once

#include <IConfigParser.h>

#include <memory>
#include <string>

class CMakeGenerator
{
public:
    /**
     * Constructs a CMakeGenerator object with the specified config parser.
     *
     * @param p A unique pointer to an IConfigParser object.
     */
    explicit CMakeGenerator(std::unique_ptr<IConfigParser> p);

    /**
     * Generates a CMakeLists.txt file based on the provided config filename.
     *
     * @param configFilename The path to the configuration file.
     *
     * @throws std::runtime_error If the CMakeLists.txt file fails to open for writing.
     */
    void generate(const std::string& configFilename);

private:
    void writeCMakeHeader(std::ofstream& file, const std::string& projectName);

    void processSections(std::ofstream& file, const std::vector<ConfigSection>& sections);
    void processDirectoriesSection(std::ofstream& file, const ConfigSection& section);
    void processExcludesSection(std::ofstream& file, const ConfigSection& section);
    void processIncludesSection(std::ofstream& file, const ConfigSection& section);
    void processDependenciesSection(std::ofstream& file, const ConfigSection& section);
    void processDefinesSection(std::ofstream& file, const ConfigSection& section);
    void processAppSection(std::ofstream& file);
    void processStaticSection(std::ofstream& file);
    void processDllSection(std::ofstream& file);

private:
    std::unique_ptr<IConfigParser> m_parser;
};