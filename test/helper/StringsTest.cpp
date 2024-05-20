/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringsTest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:45:03 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/19 18:13:36 by gasouza          ###   ########.fr       */
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