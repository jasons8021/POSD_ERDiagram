#include "PasteComponentCmdTest.h"

void PasteComponentCmdTest::SetUp()
{
	_erModel = new ERModel();

	_erModel->setComponentID(0);
	_erModel->setIsModify(false);

	const int testNum = 10;
	const string testType[testNum] = {"A", "E", "R", "A", "A", "A", "E", "R", "E", "E"};
	const string testText[testNum] = {"A0", "E1", "R2", "A3", "A4", "A5", "E6", "R7", "E8", "E9"};

	// 新增component進vector中
	for (int i = 0; i < testNum; i++)
	{
		if (testType[i] == PARAMETER_ATTRIBUTE)
			_erModel->_components.push_back(new NodeAttribute(i, testText[i], 0, 0));
		else if (testType[i] == PARAMETER_ENTITY)
			_erModel->_components.push_back(new NodeEntity(i, testText[i], 0, 0));
		else
			_erModel->_components.push_back(new NodeRelationship(i, testText[i], 0, 0));
		_erModel->_componentID++;
	}

	_erModel->addConnection(10, 0, 1, PARAMETER_NULL);
	_erModel->_componentID++;

	_pasteComponentCmd = new PasteComponentCmd(_erModel);
}

void PasteComponentCmdTest::TearDown()
{
	delete _erModel;
	delete _pasteComponentCmd;
}

TEST_F(PasteComponentCmdTest, execute)
{
	// 只放Connector
	_erModel->_clipboard.push_back(_erModel->_components[10]->deepClone());

	_pasteComponentCmd->execute();
	EXPECT_EQ(11, _erModel->_components.size());
	EXPECT_EQ(0, _pasteComponentCmd->_pasteComponentID.size());
	_erModel->resetClipboard();

	// 放一個Component及Connector
	_erModel->_clipboard.push_back(_erModel->_components[0]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[10]->deepClone());
	_pasteComponentCmd->execute();
	EXPECT_EQ(12, _erModel->_components.size());
	EXPECT_EQ("A0", _erModel->_components[11]->getText());
	EXPECT_EQ(11, _pasteComponentCmd->_pasteComponentID[0]);
	EXPECT_EQ(1, _pasteComponentCmd->_pasteComponentID.size());
	_erModel->resetClipboard();

	// 測試加入pasteComponentID的部分
	_erModel->_clipboard.push_back(_erModel->_components[9]->deepClone());
	_pasteComponentCmd->_isRedo = true;
	_pasteComponentCmd->execute();
	EXPECT_EQ(13, _erModel->_components.size());
	EXPECT_EQ("E9", _erModel->_components[12]->getText());
	EXPECT_EQ(1, _pasteComponentCmd->_pasteComponentID.size());
	_erModel->resetClipboard();
	_pasteComponentCmd->_isRedo = false;

	// component部分不動，將recoredIDBoard及pasteComponentID復原成正常型態
	_erModel->_recoredIDBoard.push_back(make_pair(0, 11));
	_erModel->_recoredIDBoard.push_back(make_pair(9, 12));
	_pasteComponentCmd->_pasteComponentID.push_back(12);

	// 完整
	_erModel->_clipboard.push_back(_erModel->_components[1]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[10]->deepClone());
	_pasteComponentCmd->execute();
	EXPECT_EQ(15, _erModel->_components.size());
	EXPECT_EQ(11, static_cast<Connector*>(_erModel->_components[14])->getSourceNodeID());
	EXPECT_EQ(13, static_cast<Connector*>(_erModel->_components[14])->getDestinationNodeID());

	// 測試pasteComponentID是否有被排序
	EXPECT_EQ(14, _pasteComponentCmd->_pasteComponentID[0]);
	EXPECT_EQ(11, _pasteComponentCmd->_pasteComponentID[1]);
	EXPECT_EQ(12, _pasteComponentCmd->_pasteComponentID[2]);
	EXPECT_EQ(13, _pasteComponentCmd->_pasteComponentID[3]);
}

TEST_F(PasteComponentCmdTest, unexecute)
{
	// 完整
	_erModel->_clipboard.push_back(_erModel->_components[0]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[9]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[1]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[10]->deepClone());
	_pasteComponentCmd->execute();

	EXPECT_EQ(15, _erModel->_components.size());
	EXPECT_EQ(11, static_cast<Connector*>(_erModel->_components[14])->getSourceNodeID());
	EXPECT_EQ(13, static_cast<Connector*>(_erModel->_components[14])->getDestinationNodeID());

	// 測試pasteComponentID是否有被排序
	EXPECT_EQ(14, _pasteComponentCmd->_pasteComponentID[0]);
	EXPECT_EQ(11, _pasteComponentCmd->_pasteComponentID[1]);
	EXPECT_EQ(12, _pasteComponentCmd->_pasteComponentID[2]);
	EXPECT_EQ(13, _pasteComponentCmd->_pasteComponentID[3]);

	_pasteComponentCmd->unexecute();

 	EXPECT_EQ(11, _erModel->_components.size());
 	EXPECT_EQ(11, _erModel->_componentID);
}
