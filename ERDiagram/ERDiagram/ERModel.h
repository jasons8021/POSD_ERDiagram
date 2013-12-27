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
#define SEMICOLON ";"
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

#define PARAMETER_NOTFINDID -1

#define PARAMETER_ADJUSTSTRING -1

#define PARAMETER_ADJUSTPOSITION 10

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest_prod.h> 
#include <direct.h>

#include "Component.h"
#include "ComponentFactory.h"

#include "Toolkit.h"
#include "Subject.h"
#include "Observer.h"

using namespace std;

class ERModel : public Subject
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
	FRIEND_TEST(ERModelTest, searchComponent);
	FRIEND_TEST(ERModelTest, searchComponentConnection);
	FRIEND_TEST(ERModelTest, searchSpecificTypeComponentSet);
	FRIEND_TEST(ERModelTest, getComponentDataList);
	FRIEND_TEST(ERModelTest, searchConnection);
	FRIEND_TEST(ERModelTest, searchRelatedComponent);
	FRIEND_TEST(ERModelTest, resetERModel);
	FRIEND_TEST(ERModelTest, sortCompoentsAndConnection);
	FRIEND_TEST(ERModelTest, getComponentForGUI);
	FRIEND_TEST(ERModelTest, changeText);
	FRIEND_TEST(ERModelTest, changePrimaryKey);
	FRIEND_TEST(ERModelTest, copyComponent);
	FRIEND_TEST(ERModelTest, cloneItemIntoClipboard);
 	FRIEND_TEST(ERModelTest, connectionSetting);
	FRIEND_TEST(ERModelTest, resetClipboard);
	FRIEND_TEST(ERModelTest, getTargetAttributeIsPrimaryKey);

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
 	FRIEND_TEST(PresentationModelTest, getComponent_GUI);
	FRIEND_TEST(PresentationModelTest, addNodeCmd);
	FRIEND_TEST(PresentationModelTest, addConnectionCmd);
	FRIEND_TEST(PresentationModelTest, deleteCmd);
	FRIEND_TEST(PresentationModelTest, undoCmd);
	FRIEND_TEST(PresentationModelTest, redoCmd);
 	FRIEND_TEST(PresentationModelTest, changePrimaryKeyCmd);
 	FRIEND_TEST(PresentationModelTest, changeTextCmd);
// 	FRIEND_TEST(PresentationModelTest, );
public:
	ERModel();
	virtual ~ERModel();
	int addNode(string, string, int, int);
	int addNode(int, string, string, int, int);
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
	void connectionSetting(Component* sourceNode, Component* destinationNode, string text);
	void recoveryFile(vector<vector<string>>);
	void recoveryAllComponent(vector<string>, vector<string>);
	void recoveryPrimaryKey(vector<string>);
	void deleteFunction(int);
	void deleteTableSet(int, vector<Component*>, int);
	void sortCompoentsAndConnection();
	void setComponentID(int);
	void setIsModify(bool);
	void deleteConnection(Component*);
	void notifyTextChanged(int, string);
	void changeText(int, string);
	void changePrimaryKey(int, bool);
	void copyComponent(vector<int>);
	void cloneItemIntoClipboard(vector<int>);
	void resetClipboard();
	bool searchComponentExist(string, string);
	bool connectedItself(Component*, Component*);
	bool connectedTypeCheck(Component*, Component*);
	bool connectedAlready(Component*, Component*);
	bool checkSetCardinality(int, int);
	bool searchComponentConnection(int, string, vector<Component*>);
	bool searchEntityConnection(int, int, string);
	bool checkOneToOne();
	bool getIsModify();
	bool getTargetAttributeIsPrimaryKey(int);
	string deleteComponent(Component*);
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
	vector<Component*> getClipboard();
	vector<vector<string>> classifyInputFile(string);
	pair<string,vector<string>> splitter(string);

	// GUI function
	string getComponentForGUI();
	string getConnectionForGUI();
	string getPrimaryKeyForGUI();

	///////////////////////////////////////////////////////////////////
	vector<int> arrangeConnectorFirst(vector<int>);
	void recordNewCloneComponentID(int, int);
	void movedComponentPosition(int, int, int);
	int addClone(Component*);
	int addCloneComponent(Component*);
	int addCloneConnection(Component*);
	int retrieveNewCloneID(int);
	///////////////////////////////////////////////////////////////////
private:
	int _componentID;
	bool _isModify;
	vector<Component*> _components;
	vector<Component*> _connections;
	vector<Component*> _clipboard;
	vector<pair<int,int>> _recoredIDBoard;
};

#endif