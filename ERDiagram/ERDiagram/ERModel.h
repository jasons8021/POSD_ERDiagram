#ifndef ERMODEL_H_
#define ERMODEL_H_

#define TEXT_CONNECTION_CANCONNECT "CANCONNECT"

#define TEXT_GETERDIAGRAM_ENDKEY ")"
#define TEXT_GETERDIAGRAM_PK "PK("
#define TEXT_GETERDIAGRAM_FK ", FK("

#define TEXT_LOADSAVE_OPENSUCCESS "The ER diagram is displayed as follows:\n"
#define TEXT_LOADSAVE_OPENFAILED "File not found!!\n"

#define TEXT_ONESPACE " "
#define TEXT_TWOSPACE "  "
#define TEXT_FIVESPACE "     "
#define TEXT_SPACELINE "   |"
#define TEXT_LINESPACE "|  "
#define TEXT_COMMASPACE ", "
#define TEXT_ENDLINE "\n"
#define COMMA ","
#define SPLITTERBYBACKSLASH "\\"

#define PARAMETER_NULL ""
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_RELATIONSHIPLOWERBOUND 2

#define PARAMETER_COMPONENTSTABLE 0
#define PARAMETER_CONNECTIONSTABLE 1

#define PARAMETER_STRINGBEGIN 0
#define PARAMETER_ADJUSTSTRINGSIZE 2

#define PARAMETER_FILETEXT_COMPONENTID 0
#define PARAMETER_FILETEXT_RELATEDCOMPONENTID 1

#define PARAMETER_SOURCENODE 0
#define PARAMETER_DESTINATIONNODE 1

#define PARAMETER_TYPE 0
#define PARAMETER_TEXT 1

#define PARAMETER_HAVETYPEANDTEXT 1

#define PARAMETER_COMPONENTSET 0
#define PARAMETER_CONNECTIONSET 1
#define PARAMETER_PRIMARYKEYSET 2

#define PARAMETER_FIRSTENTITY 0
#define PARAMETER_SECONDENTITY 1

#define PARAMETER_ZEROCOMPONENT 0
#define PARAMETER_FILLUPSIZE 10
#define PARAMETER_SEARCHNUMBEGIN 0

#define PARAMETER_ISERROR -1

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest_prod.h> 
#include <direct.h>
#include "AddComponentCmd.h"
#include "ConnectComponentsCmd.h"
#include "DeleteComponentCmd.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "CommandManager.h"
#include "Toolkit.h"

using namespace std;

class AddComponentCmd;
class ConnectComponentsCmd;
class DeleteComponentCmd;

class ERModel
{
	friend class ERModelTest;
	FRIEND_TEST(ERModelTest, addNode);
	FRIEND_TEST(ERModelTest, addConnection);
	FRIEND_TEST(ERModelTest, checkConnectionState);
	FRIEND_TEST(ERModelTest, getCheckConnectionStateMessage);
	FRIEND_TEST(ERModelTest, setCardinality);
	FRIEND_TEST(ERModelTest, getComponentsTable);
	FRIEND_TEST(ERModelTest, setPrimaryKey);
	FRIEND_TEST(ERModelTest, reBuildPrimaryKeyFromAttribute);
	FRIEND_TEST(ERModelTest, getERDiagramTable);
	FRIEND_TEST(ERModelTest, getAttributeContents);
	FRIEND_TEST(ERModelTest, setForeignKey);
	FRIEND_TEST(ERModelTest, oneToOne);
	FRIEND_TEST(ERModelTest, searchForeignKey);
	FRIEND_TEST(ERModelTest, loadERDiagram);
	FRIEND_TEST(ERModelTest, recoveryFile);
	FRIEND_TEST(ERModelTest, recoveryAllComponent);
	FRIEND_TEST(ERModelTest, recoveryConnection);
	FRIEND_TEST(ERModelTest, recoveryPrimaryKey);
	FRIEND_TEST(ERModelTest, saveERDiagram);
	FRIEND_TEST(ERModelTest, savePrimaryKeyTable);
	FRIEND_TEST(ERModelTest, deleteFunction);
	FRIEND_TEST(ERModelTest, deleteComponent);
	FRIEND_TEST(ERModelTest, deleteConnection);
	FRIEND_TEST(ERModelTest, deleteTableSet);
	FRIEND_TEST(ERModelTest, addNodeCmd);
	FRIEND_TEST(ERModelTest, addConnectionCmd);
	FRIEND_TEST(ERModelTest, deleteCmd);
	FRIEND_TEST(ERModelTest, undoCmd);
	FRIEND_TEST(ERModelTest, redoCmd);
	FRIEND_TEST(ERModelTest, searchComponent);
	FRIEND_TEST(ERModelTest, searchComponentConnection);
	FRIEND_TEST(ERModelTest, searchSpecificTypeComponentSet);
	FRIEND_TEST(ERModelTest, getComponentDataList);
	FRIEND_TEST(ERModelTest, searchConnection);
	FRIEND_TEST(ERModelTest, searchRelatedComponent);
	FRIEND_TEST(ERModelTest, resetERModel);
	FRIEND_TEST(ERModelTest, sortCompoentsAndConnection);

	friend class AddComponentCmdTest;
	FRIEND_TEST(AddComponentCmdTest, execute);
	FRIEND_TEST(AddComponentCmdTest, unexecute);

	friend class ConnectComponentsCmdTest;
	FRIEND_TEST(ConnectComponentsCmdTest, execute);
	FRIEND_TEST(ConnectComponentsCmdTest, unexecute);

	friend class DeleteComponentCmdTest;
	FRIEND_TEST(DeleteComponentCmdTest, execute);
	FRIEND_TEST(DeleteComponentCmdTest, unexecute);

	friend class CommandManagerTest;
	FRIEND_TEST(CommandManagerTest, execute);

	friend class PresentationModelTest;
	FRIEND_TEST(PresentationModelTest, getConnectionTable);
	FRIEND_TEST(PresentationModelTest, getConnectionTableSize);
	FRIEND_TEST(PresentationModelTest, searchAttributeOfEntity);
	FRIEND_TEST(PresentationModelTest, displayConnectionTable);
	FRIEND_TEST(PresentationModelTest, displayComponentTable);
	FRIEND_TEST(PresentationModelTest, displayAttributeTable);
	FRIEND_TEST(PresentationModelTest, checkAttributeInEntity);
	FRIEND_TEST(PresentationModelTest, setPrimaryKeys);
 	FRIEND_TEST(PresentationModelTest, displayERDiagramTable);
// 	FRIEND_TEST(PresentationModelTest, );
// 	FRIEND_TEST(PresentationModelTest, );
// 	FRIEND_TEST(PresentationModelTest, );
// 	FRIEND_TEST(PresentationModelTest, );
// 	FRIEND_TEST(PresentationModelTest, );
// 	FRIEND_TEST(PresentationModelTest, );
public:
	ERModel();
	virtual ~ERModel();
	int addNode(string, string);
	int addNode(int, string, string);
	int addConnection(int, int, string);
	int addConnection(int, int, int, string);
	int getComponentTableSize();
	int getConnectionTableSize();
	int getComponentID();
	int recoveryConnection(vector<string>, int, string);
	void resetERModel();
	void setCardinality(Component*, Component*, string);
	void setPrimaryKey(int, vector<int>);
	void reBuildPrimaryKeyFromAttribute(int, int);
	void setForeignKey();
	void saveERDiagram(string);
	void creatFilePath(string);
	void recoveryFile(vector<vector<string>>);
	void recoveryAllComponent(vector<string>, vector<string>);
	void recoveryPrimaryKey(vector<string>);
	void deleteFunction(int);
	void deleteComponent(Component*);
	void deleteTableSet(int, vector<Component*>, int);
	void deleteConnection(Component*);
	void addNodeCmd(string, string);
	void addConnectionCmd(int, int, string);
	void deleteCmd(int);
	void sortCompoentsAndConnection();
	void setComponentID(int);
	void setIsModify(bool);
	bool undoCmd();
	bool redoCmd();
	bool searchComponentExist(string, string);
	bool connectedItself(Component*, Component*);
	bool connectedTypeCheck(Component*, Component*);
	bool connectedAlready(Component*, Component*);
	bool checkSetCardinality(int, int);
	bool searchComponentConnection(int, string, vector<Component*>);
	bool searchEntityConnection(int, int, string);
	bool checkOneToOne();
	bool getIsModify();
	string loadERDiagram(string);
	string checkConnectionState(Component*, Component*);
	string getCheckConnectionStateMessage(int, int);
	string getComponentsTable(string);
	string getComponentDataList(string, vector<Component*>);
	string getConnectionTable();
	string getERDiagramTable();
	string searchAttributeOfEntity(int);
	string getAttributeContents(vector<Component*>);
	string searchForeignKey(int);
	string saveComponentTable();
	string saveConnectionTable();
	string savePrimaryKeyTable();
	Component* searchComponent(int);
	vector<int> oneToOne(NodeRelationship*);
	vector<Component*> searchSpecificTypeComponentSet(string, vector<Component*>);
	vector<Component*> searchConnection(int);
	vector<Component*> searchRelatedComponent(int);
	vector<vector<string>> classifyInputFile(string);
	pair<string,vector<string>> splitter(string);
private:
	int _componentID;
	bool _isModify;
	CommandManager _cmdManager;
	vector<Component*> _components;
	vector<Component*> _connections;
};

#endif