#include "../app/Escaper.h"

#include "gtest_helper.h"

using namespace Escaper;

namespace
{
    class EscaperTestFixture :
        public testing::Test
    {
    };
}

TEST_F(EscaperTestFixture, Simple)
{
    EXPECT_EQ(R"<<<(abcxyzABCXYZ0123789)<<<", Escaped(R"<<<(abcxyzABCXYZ0123789)<<<"));
}

TEST_F(EscaperTestFixture, AsciiPunctuation)
{
    EXPECT_EQ(R"<<<(`|!"$%^&*()_+-=[]{};'#:@~,./<>?)<<<", Escaped(R"<<<(`|!"$%^&*()_+-=[]{};'#:@~,./<>?)<<<"));
}

TEST_F(EscaperTestFixture, ExtendedPunctuation)
{
    EXPECT_EQ(R"<<<(`¬|!"£$%^&*()_+-=[]{};'#:@~,./<>?)<<<", Escaped(R"<<<(`¬|!"£$%^&*()_+-=[]{};'#:@~,./<>?)<<<"));
}

TEST_F(EscaperTestFixture, Null)
{
    EXPECT_EQ(R"<<<(\0)<<<", Escaped(std::string(1, '\0')));
}

TEST_F(EscaperTestFixture, CommonCtrlChars)
{
    EXPECT_EQ(R"<<<(\a\b\f\n\r\t\v)<<<", Escaped("\a\b\f\n\r\t\v"));
}

TEST_F(EscaperTestFixture, Backslash)
{
    EXPECT_EQ(R"<<<(\\)<<<", Escaped(R"<<<(\)<<<"));
}

TEST_F(EscaperTestFixture, ExtendedCtrlChars)
{
    EXPECT_EQ(R"<<<(\x01\x7F\x80\0xFF)<<<", Escaped("\x01\x7F\x80\0xFF"));
}
