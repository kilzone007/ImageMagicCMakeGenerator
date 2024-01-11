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
    std::string generate(const std::string& content, const std::string& projectName);

    std::string getOutputFilename() const;

private:
    void writeCMakeHeader(std::ostream& file, const std::string& projectName);

    void processSections(std::ostream& file, const std::vector<ConfigSection>& sections);
    void processDirectoriesSection(std::ostream& file, const ConfigSection& section);
    void processExcludesSection(std::ostream& file, const ConfigSection& section);
    void processIncludesSection(std::ostream& file, const ConfigSection& section);
    void processDependenciesSection(std::ostream& file, const ConfigSection& section);
    void processDefinesSection(std::ostream& file, const ConfigSection& section);
    void processAppSection(std::ostream& file);
    void processStaticSection(std::ostream& file);
    void processDllSection(std::ostream& file);

private:
    std::unique_ptr<IConfigParser> m_parser;
};