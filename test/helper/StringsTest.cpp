/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringsTest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:45:03 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/30 00:50:41 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/helper/Strings.hpp"


TEST(StringsTest, ReplaceMultiplesSpaces)
{
    std::string str = "        ";
    Strings::replace(str, "  ", " ");

    EXPECT_EQ(" ", str);
}

TEST(StringsTest, ReplaceWithMultiplesSizes)
{
    std::string str = "abc 54 *&\n -------       ";
    
    Strings::replace(str, "abc", "");
    Strings::replace(str, "54", "");
    Strings::replace(str, "*&\n", "");
    Strings::replace(str, "-", "");
    Strings::replace(str, " ", "");

    Strings::replace(str, "", "empty should not replace");

    EXPECT_EQ("", str);

    str = " -- ";
    
    Strings::replace(str, "-", "a long word to add ");

    EXPECT_EQ(" a long word to add a long word to add  ", str);
}

TEST(StringsTest, SplitWithoutDelimiterInIt)
{
    std::string str = " my string without split delimiter ";

    std::vector<std::string> tokens = Strings::split(str, ":");

    EXPECT_EQ(1, tokens.size());
    EXPECT_EQ(str, tokens.at(0));
}

TEST(StringsTest, SplitWithDelimiterInIt)
{
    std::string str = " my string with : split delimiter : in it ";

    std::vector<std::string> tokens = Strings::split(str, ":");

    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(" my string with ", tokens.at(0));
    EXPECT_EQ(" split delimiter ", tokens.at(1));
    EXPECT_EQ(" in it ", tokens.at(2));
}

TEST(StringsTest, SplitWithDelimiterAtEnd)
{
    std::string str = "val1,val2,val3,";

    std::vector<std::string> tokens = Strings::split(str, ",");

    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("val1", tokens.at(0));
    EXPECT_EQ("val2", tokens.at(1));
    EXPECT_EQ("val3", tokens.at(2));
}

TEST(StringsTest, ToUpper)
{
    EXPECT_EQ("", Strings::toUpper(""));
    EXPECT_EQ("1234", Strings::toUpper("1234"));
    EXPECT_EQ(" RGHT ", Strings::toUpper(" rght "));
    EXPECT_EQ("@#T A B T &! ", Strings::toUpper("@#t a b t &! "));
}

TEST(StringsTest, stringsIsOnPattern)
{
    EXPECT_TRUE(Strings::isOnPattern("BrunoLuiz1234", ALPHANUM_PATTERN));
    EXPECT_TRUE(Strings::isOnPattern("BrunoLuiz1234@#!", ALPHANUM_PATTERN, 12));
}

TEST(StringsTest, stringIsNotOnPattern)
{
    EXPECT_FALSE(Strings::isOnPattern("BrunoLuiz1234@#$%", ALPHANUM_PATTERN, 16));
    EXPECT_FALSE(Strings::isOnPattern("\"BrunoLuiz1234", ALPHANUM_PATTERN));
    EXPECT_FALSE(Strings::isOnPattern("BrunoL\ruiz1234", ALPHANUM_PATTERN));
}

TEST(StringsTest, stringEmptyAndOrPatternEmpty)
{
	std::string str1 = "";
	std::string str2 = "BrunoLuiz1234";
	std::string pattern1 = "";
	std::string pattern2 = ALPHANUM_PATTERN;

    EXPECT_FALSE(Strings::isOnPattern("", ""));
    EXPECT_FALSE(Strings::isOnPattern("", "", 50));
    EXPECT_FALSE(Strings::isOnPattern("BrunoLuiz1234", ""));
    EXPECT_FALSE(Strings::isOnPattern("BrunoLuiz1234", "", 2));
    EXPECT_FALSE(Strings::isOnPattern("", ALPHANUM_PATTERN));
    EXPECT_FALSE(Strings::isOnPattern("", ALPHANUM_PATTERN, -1));
    EXPECT_FALSE(Strings::isOnPattern("", ALPHANUM_PATTERN, 2.5f));
}

TEST(StringsTest, truncateVariousSizes)
{
	std::string stringTest = "123456789";
	Strings::truncateBySize(stringTest, 7);
    EXPECT_EQ("1234567", stringTest);

	stringTest = "123456789";
	Strings::truncateBySize(stringTest, 3);
	EXPECT_EQ("123", stringTest);

	stringTest = "123456789";
	Strings::truncateBySize(stringTest, 1);
	EXPECT_EQ("1", stringTest);

	stringTest = "123456789";
	Strings::truncateBySize(stringTest, 0);
	EXPECT_EQ("", stringTest);

	stringTest = "123456789";
	Strings::truncateBySize(stringTest, 50);
	EXPECT_EQ("123456789", stringTest);
}

TEST(StringsTest, SplitWithEmptyArgs)
{
    std::vector<std::string> strs;
    
    EXPECT_EQ("", Strings::join(strs.begin(), strs.end(), " "));
    EXPECT_EQ("", Strings::join(strs.begin(), strs.end(), "-"));
}

TEST(StringsTest, SplitWithOneArgs)
{
    std::vector<std::string> strs;
    strs.push_back("test");
    
    EXPECT_EQ("test", Strings::join(strs.begin(), strs.end(), " "));
    EXPECT_EQ("test", Strings::join(strs.begin(), strs.end(), "-"));
}

TEST(StringsTest, SplitWithSomeArgs)
{
    std::vector<std::string> strs;
    strs.push_back("test1");
    strs.push_back("test2");
    strs.push_back("test3");
    
    EXPECT_EQ("test1 test2 test3", Strings::join(strs.begin(), strs.end(), " "));
    EXPECT_EQ("test1-test2-test3", Strings::join(strs.begin(), strs.end(), "-"));
}

TEST(StringsTest, Sprintf)
{
    EXPECT_EQ("", Strings::f("", ""));
    EXPECT_EQ("teste teste", Strings::f("teste %s", "teste"));
    EXPECT_EQ("teste teste teste", Strings::f("teste %s %s", "teste", "teste"));
    EXPECT_EQ("teste teste teste teste", Strings::f("teste %s %s %s", "teste", "teste", "teste"));
}