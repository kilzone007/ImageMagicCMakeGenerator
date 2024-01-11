#include <ArgParser.h>
#include <CustomConfigParser.h>
#include <CMakeGenerator.h>

#include <FileReader.h>
#include <FileSaver.h>

#include <iostream>
#include <string>
#include <filesystem>


void printUsage(const char* programName)
{
    std::cout << "Usage: " << programName << " [-h] [-f filename]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h\t\tShow this help message" << std::endl;
    std::cout << "  -f filename\tSpecify the input file" << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        ArgParser parser(argc, argv);

        if (parser.hasOption("-h") || !parser.hasOption("-f"))
        {
            printUsage(argv[0]);
            return 0;
        }

        std::string filename = parser.getOptionValue("-f");
        if (!filename.empty())
        {
            CMakeGenerator generator(std::make_unique<CustomConfigParser>());

            for (const auto& entry : std::filesystem::recursive_directory_iterator(filename))
            {
                if (entry.is_regular_file() && entry.path().filename() == "Config.txt")
                {
                    std::cout << "Parsing file: " << entry.path() << std::endl;

                    auto path = entry.path();

                    FileReader reader(entry.path());
                    const auto content = generator.generate(reader.readFile(), path.parent_path().filename().string());

                    FileSaver saver(path.replace_filename(generator.getOutputFilename()).string());\
                    saver.saveToFile(content);

                    std::cout << "CMakeLists.txt has been generated successfully!" << std::endl;
                }
            }
        }
        else
        {
            printUsage(argv[0]);
            return 0;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Main Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}