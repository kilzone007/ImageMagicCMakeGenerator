#include <CMakeGenerator.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class CMakeGeneratorFixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        sections.emplace_back("STATIC", std::vector<std::string>{});
        sections.emplace_back("DIRECTORIES", std::vector<std::string>{"dir1", "dir2"});

        content1 = R"(cmake_minimum_required(VERSION 3.10)
project(projectName)

set(${PROJECT_NAME}_SOURCES)

file(GLOB _tmp_files
${CMAKE_CURRENT_LIST_DIR}\dir1\*.c
${CMAKE_CURRENT_LIST_DIR}\dir1\*.h
${CMAKE_CURRENT_LIST_DIR}\dir1\*.cpp
${CMAKE_CURRENT_LIST_DIR}\dir1\*.hpp)
list(APPEND ${PROJECT_NAME}_SOURCES ${_tmp_files})

file(GLOB _tmp_files
${CMAKE_CURRENT_LIST_DIR}\dir2\*.c
${CMAKE_CURRENT_LIST_DIR}\dir2\*.h
${CMAKE_CURRENT_LIST_DIR}\dir2\*.cpp
${CMAKE_CURRENT_LIST_DIR}\dir2\*.hpp)
list(APPEND ${PROJECT_NAME}_SOURCES ${_tmp_files})

unset(_tmp_files)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

)";
    }

    void TearDown() override
    {

    }

    std::vector<ConfigSection> sections;
    std::string content1;
};

class MockConfigParser : public IConfigParser
{
public:
    MOCK_CONST_METHOD1(parse, std::vector<ConfigSection>(const std::string&));
};

TEST_F(CMakeGeneratorFixture, Generate)
{
    auto parser = std::make_unique<MockConfigParser>();
    EXPECT_CALL(*parser, parse(testing::_)).WillOnce(testing::Return(sections));

    CMakeGenerator generator(std::move(parser));
    auto content = generator.generate("content", "projectName");

    EXPECT_EQ(content, content1);
}
