#include "ToolkitTest.h"

TEST_F(ToolkitTest, integerToString){
	// ����
	EXPECT_EQ("10", toolkit.integerToString(10));
	// �s
	EXPECT_EQ("0", toolkit.integerToString(0));
	// �t��
	EXPECT_EQ("-1", toolkit.integerToString(-1));
}

TEST_F(ToolkitTest, splitFunction){
	vector<string> splitResult;

	// �@�ӲŸ��r��������
	splitResult = toolkit.splitFunction("hello ,Unit ,Test", " ");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ(",Unit", splitResult[1]);
	EXPECT_EQ(",Test", splitResult[2]);
	splitResult.clear();

	//�h�ӲŸ��r��������
	splitResult = toolkit.splitFunction("hello ,Unit ,Test", " ,");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ("Unit", splitResult[1]);
	EXPECT_EQ("Test", splitResult[2]);
	splitResult.clear();

	// �[�J��L�Ÿ��A�Ʀr
	splitResult = toolkit.splitFunction("hello , Unit ,  Test, 99, ,100, 101", " ,");
	EXPECT_EQ("hello", splitResult[0]);
	EXPECT_EQ(" Unit", splitResult[1]);
	EXPECT_EQ("  Test, 99,", splitResult[2]);
	EXPECT_EQ("100, 101", splitResult[3]);
	splitResult.clear();

	// �Ʀr������
	splitResult = toolkit.splitFunction("5997", "99");
	EXPECT_EQ("5", splitResult[0]);
	EXPECT_EQ("7", splitResult[1]);
	splitResult.clear();
}