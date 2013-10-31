#include "ToolkitTest.h"

TEST_F(ToolkitTest, integerToString){
	// タ计
	EXPECT_EQ("10", toolkit.integerToString(10));
	// 箂
	EXPECT_EQ("0", toolkit.integerToString(0));
	// 璽计
	EXPECT_EQ("-1", toolkit.integerToString(-1));
}

TEST_F(ToolkitTest, splitFunction){
	vector<string> splitResult;

	// 才腹じだ澄
	splitResult = toolkit.splitFunction("hello ,Unit ,Test", " ");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ(",Unit", splitResult[1]);
	EXPECT_EQ(",Test", splitResult[2]);
	splitResult.clear();

	//才腹じだ澄
	splitResult = toolkit.splitFunction("hello ,Unit ,Test", " ,");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ("Unit", splitResult[1]);
	EXPECT_EQ("Test", splitResult[2]);
	splitResult.clear();

	// ㄤ才腹计
	splitResult = toolkit.splitFunction("hello , Unit ,  Test, 99, ,100, 101", " ,");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ(" Unit", splitResult[1]);
	EXPECT_EQ("  Test, 99,", splitResult[2]);
	EXPECT_EQ("100, 101", splitResult[3]);
	splitResult.clear();

	// 计だ澄
	splitResult = toolkit.splitFunction("5997", "99");
	EXPECT_EQ("5", splitResult[0]);
	EXPECT_EQ("7", splitResult[1]);
	splitResult.clear();
}