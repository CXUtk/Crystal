#include <Interfaces/FileSystem/Path.h>

#include "../test.h"
//using namespace crystal;

TEST(Test_FileSystem, Path_Combine)
{
	//EXPECT_EQ(Path::Combine("src", "bin"), "src/bin");
	//EXPECT_EQ(Path::Combine("src/", "bin"), "src/bin");
	//EXPECT_EQ(Path::Combine("src/", "/bin"), "/bin");
	//EXPECT_EQ(Path::Combine("src//", "bin"), "src//bin");
	//EXPECT_EQ(Path::Combine("", "bin"), "bin");	
	//EXPECT_EQ(Path::Combine("src", ""), "src");

	//EXPECT_EQ(Path::Combine({ "src", "bin", "1.txt"}), "src/bin/1.txt");
	//EXPECT_EQ(Path::Combine({ "src/", "bin/", "1.txt" }), "src/bin/1.txt");
	//EXPECT_EQ(Path::Combine({ "src/", "/bin", "1.txt"}), "/bin/1.txt");
	//EXPECT_EQ(Path::Combine({ "src//", "bin", ""}), "src//bin");
	//EXPECT_EQ(Path::Combine({ "", "bin", ""}), "bin");
	//EXPECT_EQ(Path::Combine({ "src", "", ""}), "src");
}

TEST(Test_FileSystem, Path_ChangeExtension)
{
	//EXPECT_EQ(Path::ChangeExtension("src/test.cpp", "h"), "src/test.h");
	//EXPECT_EQ(Path::ChangeExtension("src/test.cpp", ".h"), "src/test.h");
	//EXPECT_EQ(Path::ChangeExtension("src/test.cpp", ""), "src/test");
	//// No extension so keep it the same
	//EXPECT_EQ(Path::ChangeExtension("src/p", ".h"), "src/p");
}
