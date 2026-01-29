#include <gtest/gtest.h>
#include "Core/StringUtils.h"

TEST(StringUtilsTests, Contains) {
    std::string hay = "Hello World";
    std::string needle = "World";
    EXPECT_TRUE(StringUtils::Contains(hay, needle));
    EXPECT_FALSE(StringUtils::Contains(hay, "Goodbye"));
    EXPECT_TRUE(StringUtils::Contains(hay, "Hello"));
}

TEST(StringUtilsTests, Trimming) {
    std::string s = "  TrimMe  ";
    StringUtils::Trim(s);
    EXPECT_EQ(s, "TrimMe");

    std::string l = "  Left";
    StringUtils::LTrim(l);
    EXPECT_EQ(l, "Left");

    std::string r = "Right  ";
    StringUtils::RTrim(r);
    EXPECT_EQ(r, "Right");
    
    std::string lOnly = "  LeftOnly  ";
    StringUtils::LTrim(lOnly);
    EXPECT_EQ(lOnly, "LeftOnly  ");

    std::string rOnly = "  RightOnly  ";
    StringUtils::RTrim(rOnly);
    EXPECT_EQ(rOnly, "  RightOnly");
}

TEST(StringUtilsTests, Replace) {
    std::string s = "one two one three";
    
    StringUtils::ReplaceFirst(s, "one", "1");
    EXPECT_EQ(s, "1 two one three");

    // Reset to have multiple to replace
    s = "one two one three";
    StringUtils::Replace(s, "one", "1");
    EXPECT_EQ(s, "1 two 1 three");
}

TEST(StringUtilsTests, Split) {
    std::string s = "a,b,c";
    auto parts = StringUtils::Split(s, ",");
    ASSERT_EQ(parts.size(), 3);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}
