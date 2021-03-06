#ifndef PRESENTATIONMODEL_H_
#define PRESENTATIONMODEL_H_

// Choice 1. Add new node text set. They make use of onShow.
#define TEXT_ADDNEWNODE_TYPE "What kind of node do you want to add?\n[A]Attribute [E]Entity [R]Relation\n> "
#define TEXT_ADDNEWNODE_ERRORTYPE "You entered an invalid node. Please enter a valid one again.\n> "
#define TEXT_ADDNEWNODE_NAME "Enter the name of this node:\n> "
#define TEXT_ADDNEWNODE_TITLE "Components:"

// Choice 2. Add connection between two node text set.
#define TEXT_CONNECTION_FIRSTNODE "Please enter the first node ID\n> "
#define TEXT_CONNECTION_SECONDNODE "Please enter the second node ID\n> "
#define TEXT_CONNECTION_ERRORNODE "The node ID you entered does not exist. Please enter a valid one again.\n> "
#define TEXT_CONNECTION_CONNECTFINISH "' has been connected to the node '"
#define TEXT_CONNECTION_NOTCONNECTION "' cannot be connected by the node '"
#define TEXT_CONNECTION_ITSELFCONNECTION "' cannot be connected to itself."
#define TEXT_CONNECTION_ALREADYCONNECTION "' has already been connected to component '"
#define TEXT_CONNECTION_CARDINALITY "Enter the type of the cardinality:"
#define TEXT_CONNECTION_CARDINALITYOPTION "[0]1 [1]N\n> "
#define TEXT_CONNECTION_CARDINALITYRELATIONSHIP "Its cardinality of the relationship is '"
#define TEXT_CONNECTION_CANCONNECT "CANCONNECT"
#define TEXT_CONNECTION_TITLE "Connections:"
#define TEXT_CONNECTION_NOCOMPONENT "There is no component to connect."

// Choice 4. SetPrimaryKey text set
#define TEXT_SETPRIMARYKEY_ERRORMESSAGE "' is not an entity. Please enter a valid one again.\n> "
#define TEXT_SETPRIMARYKEY_ENTITYTITLE "Entities:"
#define TEXT_SETPRIMARYKEY_ATTRIBUTETITLE "Attributes of the entity '"
#define TEXT_SETPRIMARYKEY_ENDTEXT "'"
#define TEXT_SETPRIMARYKEY_ENTERNODEID "Enter the ID of the entity:\n> "
#define TEXT_SETPRIMARYKEY_ATTRIBUTEOFENTITY "Attributes of the entity '"
#define TEXT_SETPRIMARYKEY_ENTERTWOATTRIBUTE "Enter the IDs of the attributes (use a comma to separate two attributes):\n> "
#define TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_ONE "' does not belong to Entity '"
#define TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_TWO "'. Please enter a valid one again.\n> "
#define TEXT_SETPRIMARYKEY_SETPKFINISH_ONE "' has the primary key ("
#define TEXT_SETPRIMARYKEY_SETPKFINISH_TWO ")."

// Choice 5. ERDiagramTable text set
#define TEXT_ERDIAGRAM_TITLE " Entity   |  Attribute"
#define TEXT_ERDIAGRAM_NOTABLE "It has no table to display."

// Choice 7. Load/Save ERDiagram
#define TEXT_LOADSAVE_FILENAME "Please input the file name: "
#define TEXT_LOADSVAE_EXITPROGRAM "You have modified the ERDiagram. Do you want to save ?"
#define TEXT_LOADSVAE_SAVEORNOT "[0]Yes [1]No"
#define TEXT_LOADSAVE_OPENFAILED "File not found!!\n"

// Choice 8. Delete
#define TEXT_DELETE_ENTERNODE "Please enter the component ID\n> "
#define TEXT_DELETE_DELETEFINISH_ONE "The component '"
#define TEXT_DELETE_DELETEFINISH_TWO "' has been deleted."

// Choice 9.undo/redo
#define TEXT_UNDO_SUCCESS "Undo succeed"
#define TEXT_REDO_SUCCESS "Redo succeed"
#define TEXT_UNDO_FAILED "Undo failed"
#define TEXT_REDO_FAILED "Redo failed"

// General text set
#define TEXT_NODENUMBEGIN "The node '"
#define TEXT_ENDTEXT "'."
#define TEXT_COMPONENT_TABLEFORMAT " TYPE |  ID  |  NAME"
#define TEXT_CONNECTION_TABLEFORMAT "Connection | node | node |"

// Demarcation text set. They make use of onShow.
#define TEXT_DEMARCATION_COMPONENTTABLE "------------------------------------"
#define TEXT_DEMARCATION_CONNECTIONTABLE "--------------------------"
#define TEXT_DEMARCATION_ERDIAGRAMTABLE "----------------------------------------------------------"
#define TEXT_DEMARCATIONTWO_CONPONENTTABLE "------+------+----------------------"
#define TEXT_DEMARCATIONTWO_CONNECTIONTABLE "-----------+------+------|"
#define TEXT_DEMARCATIONTWO_ERDIAGRAMTABLE "----------+-----------------------------------------------"
#define TEXT_TWOSPACE "  "
#define TEXT_FIVESPACE "     "
#define TEXT_SPACELINE "   |"
#define TEXT_ENDLINE "\n"

// Parameter text set. They are parameters in some method.
#define PARAMETER_ADJUSTPKSHOWSTRINGDOUBLESIZE 2
#define PARAMETER_ADJUSTPKSHOWSTRINGSUBLAST -1

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest_prod.h>

#include "ERModel.h"
#include "CommandManager.h"
#include "AddComponentCmd.h"
#include "ConnectComponentsCmd.h"
#include "DeleteComponentCmd.h"
#include "ChangeTextCmd.h"
#include "ChangePrimaryKeyCmd.h"
#include "DeleteGroupCmd.h"
#include "CutComponentCmd.h"
#include "PasteComponentCmd.h"
#include "MoveComponentCmd.h"
#include "Toolkit.h"
#include "Observer.h"

class PresentationModel
{
	friend class PresentationModelTest;
	FRIEND_TEST(PresentationModelTest, getConnectionTable);
	FRIEND_TEST(PresentationModelTest, getConnectionTableSize);
	FRIEND_TEST(PresentationModelTest, searchAttributeOfEntity);
	FRIEND_TEST(PresentationModelTest, displayComponentTable_TextUI);
	FRIEND_TEST(PresentationModelTest, displayConnectionTable_TextUI);
	FRIEND_TEST(PresentationModelTest, displayAttributeTable_TextUI);
	FRIEND_TEST(PresentationModelTest, checkAttributeInEntity_TextUI);
	FRIEND_TEST(PresentationModelTest, setPrimaryKeys_TextUI);
 	FRIEND_TEST(PresentationModelTest, displayERDiagramTable_TextUI);
 	FRIEND_TEST(PresentationModelTest, deleteComponent);
 	FRIEND_TEST(PresentationModelTest, getComponent_GUI);
 	FRIEND_TEST(PresentationModelTest, getConnection_GUI);
 	FRIEND_TEST(PresentationModelTest, getPrimaryKeySet_GUI);
	FRIEND_TEST(PresentationModelTest, addConnectionCmd);
	FRIEND_TEST(PresentationModelTest, deleteCmd);
	FRIEND_TEST(PresentationModelTest, undoCmd);
	FRIEND_TEST(PresentationModelTest, redoCmd);
public:
	PresentationModel(ERModel*);
	virtual ~PresentationModel();
	int getComponentTableSize();
	int getConnectionTableSize();
	bool addNodeCmd_TextUI(string, string, int, int);
	bool checkSetCardinality(int, int);
	bool getIsModify();
	void saveERDiagram(string);
	void saveXmlComponent(string);
	string addConnectionCmd_TextUI(int, int, string);
	string displayComponentTable_TextUI();
	string displayConnectionTable_TextUI();
	string searchComponent_TextUI(string, string);
	string searchEntity_TextUI(string);
	string displayEntityTable_TextUI();
	string displayAttributeTable_TextUI(int);
	string checkAttributeInEntity_TextUI(string, vector<int>);
	string setPrimaryKeys_TextUI(int, vector<int>);
	string displayERDiagramTable_TextUI();
	string loadERDiagram_TextUI(string);
	string deleteComponent_TextUI(string);
	string undo_TextUI();
	string redo_TextUI();
	vector<int> splitPrimaryKey(string);
	// Command Pattern
	void addNodeCmd(string, string, int, int);
	void addConnectionCmd(int, int, string);
	void deleteCmd(int);
	void changeTextCmd(int, string);
	void changePrimaryKeyCmd(int, bool);
	void deleteGroupCmd(vector<int>);
	void cutComponentCmd(vector<int>);
	void copyComponent(vector<int>);
	void pasteComponentCmd();
	void moveItemCmd(vector<int>, int, int);
	bool undoCmd();
	bool redoCmd();
	bool checkOneToOne();
	int getUndoCmdsSize();
	int getRedoCmdsSize();
	// GUI Function
	string getComponent_GUI();
	string getConnection_GUI();
	string getPrimaryKeySet_GUI();
	string getHTMLERDiagramTable();
	bool addConnectionCmd_GUI(int, int, string);
	bool getPosFileExist();
	int getComponentID();
	bool getTargetAttributeIsPrimaryKey(int);
	void setInitialItemPosition(int, int, int);
	void setInitialPrimaryKey(int, bool);
	vector<int> getTargetPosition(int);
	// ObserverPattern
	void attachObserver(Observer*);
	void detachObserver(Observer*);

	void clearERModelComponent();

private:
	ERModel* _erModel;
	CommandManager _cmdManager;
};

#endif
