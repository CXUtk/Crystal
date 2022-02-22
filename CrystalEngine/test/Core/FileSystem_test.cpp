#include <Interfaces/FileSystem/Path.h>

#include "../test.h"
using namespace crystal;

TEST(Test_FileSystem, Path_Combine)
{
	EXPECT_EQ(Path::Combine("src", "bin"), "src/bin");
	EXPECT_EQ(Path::Combine("src/", "bin"), "src/bin");
	EXPECT_EQ(Path::Combine("src/", "/bin"), "/bin");
	EXPECT_EQ(Path::Combine("src//", "bin"), "src//bin");
	EXPECT_EQ(Path::Combine("", "bin"), "bin");	
	EXPECT_EQ(Path::Combine("src", ""), "src");

	EXPECT_EQ(Path::Combine({ "src", "bin", "1.txt"}), "src/bin/1.txt");
	EXPECT_EQ(Path::Combine({ "src/", "bin/", "1.txt" }), "src/bin/1.txt");
	EXPECT_EQ(Path::Combine({ "src/", "/bin", "1.txt"}), "/bin/1.txt");
	EXPECT_EQ(Path::Combine({ "src//", "bin", ""}), "src//bin");
	EXPECT_EQ(Path::Combine({ "", "bin", ""}), "bin");
	EXPECT_EQ(Path::Combine({ "src", "", ""}), "src");
}
