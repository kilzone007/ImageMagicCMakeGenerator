#include <CustomConfigParser.h>

#include <gtest/gtest.h>

#include <ranges>
#include <functional>

class ConfigFixture : public testing::Test
{
public:
    void SetUp() override
    {
        content = R"([DLL]

[LICENSE]
COPYING-LGPL-2.1

[DEFINES]
CAIRO_FEATURES_H
CAIRO_COMPILATION

[DIRECTORIES]
cairo\src
cairo\src\win32

[INCLUDES]
freetype\include
glib
pixman\pixman
png
zlib

[EXCLUDES]
check-link.c
cairo-dwrite-font.cpp
cairo-image-mask-compositor.c
cairo-quartz*
cairo-tee*
cairo-xcb*
cairo-xlib*
check-has-hidden-symbols.c

[DEPENDENCIES]
freetype
glib
pixman
png
zlib

[CONFIG_DEFINE]
/*
  Define to use the CAIRO library
*/
#define MAGICKCORE_CAIRO_DELEGATE)";
    }

    void TearDown() override
    {

    }

    std::string content;
};

TEST_F(ConfigFixture, Parse)
{
    auto parser = std::make_unique<CustomConfigParser>();
    auto sections = parser->parse(content);

    auto findSectionByName = [&sections](const std::string& name)
    {
        return std::ranges::find_if(sections, [&name](const ConfigSection& section)
        {
            return section.name == name;
        });
    };

    EXPECT_EQ(sections.size(), 8);

    auto dllSection = findSectionByName("DLL");

    EXPECT_NE(dllSection, sections.end());
    EXPECT_STREQ(dllSection->name.c_str(), "DLL");
    EXPECT_EQ(dllSection->lines.size(), 0);

    auto directoriesSection = findSectionByName("DIRECTORIES");

    EXPECT_NE(directoriesSection, sections.end());
    EXPECT_EQ(directoriesSection->lines.size(), 2);
}