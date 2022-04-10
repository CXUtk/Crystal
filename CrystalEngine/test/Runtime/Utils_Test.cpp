#include "Core/Utils/Misc.h"

#include "../test.h"

using namespace crystal;

TEST(Test_Utils_Misc, String_Trim)
{
	EXPECT_EQ(Trim("abcd"), "abcd");
	EXPECT_EQ(Trim("a  "), "a");
	EXPECT_EQ(Trim("  a  "), "a");
	EXPECT_EQ(Trim("  ab cd  "), "ab cd");
	EXPECT_EQ(Trim("    "), "");
}
