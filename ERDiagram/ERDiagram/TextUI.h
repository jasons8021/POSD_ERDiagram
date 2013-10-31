#ifndef TEXTUI_H_
#define TEXTUI_H_

// Menu text set.
#define TEXT_MENU "1.  Load ER diagram file\n2.  Save ER diagram file\n3.  Add a node\n4.  Connect two nodes\n5.  Display the current diagram\n6.  Set a primary key\n7.  Display the table\n8.  Delete a component\n9.  Undo\n10. Redo\n11. Exit\n> "
#define TEXT_MENU_ERRORCHOICE "You entered a invalid number. Please enter a valid number again.\n> "

// General text set
#define TEXT_NODENUMBEGIN "The node '"
#define TEXT_ENDTEXT "'."
#define TEXT_COMPONENT_TABLEFORMAT " TYPE |  ID  |  NAME"
#define TEXT_CONNECTION_TABLEFORMAT "Connection | node | node |"

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

// Choice 6. GoodBye
#define TEXT_GOODBYE "Goodbye!"

// Choice 7. Load/Save ERDiagram
#define TEXT_LOADSAVE_FILENAME "Please input the file name: "
#define TEXT_LOADSVAE_EXITPROGRAM "You have modified the ERDiagram. Do you want to save ?"
#define TEXT_LOADSVAE_SAVEORNOT "[0]Yes [1]No"

// Choice 8. Delete
#define TEXT_DELETE_ENTERNODE "Please enter the component ID\n> "
#define TEXT_DELETE_DELETEFINISH_ONE "The component '"
#define TEXT_DELETE_DELETEFINISH_TWO "' has been deleted."

// Choice 9.undo/redo
#define TEXT_UNDO_SUCCESS "Undo succeed"
#define TEXT_REDO_SUCCESS "Redo succeed"
#define TEXT_UNDO_FAILED "Undo failed"
#define TEXT_REDO_FAILED "Redo failed"

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

// Parameter text set. They are parameters in some method.
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_CARDINALITYOPTION_ONE "1"
#define PARAMETER_CARDINALITYOPTION_TWO "N"
#define PARAMETER_INITIALSEARCH "initial search"
#define PARAMETER_ADJUSTPKSHOWSTRINGDOUBLESIZE 2
#define PARAMETER_ADJUSTPKSHOWSTRINGSUBLAST -1
#define PARAMETER_ZEROCOMPONENT 0
#define PARAMETER_SAVEEXIT 0
#define PARAMETER_NOSAVEEXIT 1

#define COMMA ","
#define GETINPUT "> "

#include <iostream>
#include <sstream>
#include <string>
#include "ERModel.h"
#include "Component.h"
#include "Connector.h"
#include "Toolkit.h"
#include "PresentationModel.h"

using namespace std;

enum Option{Load = 1, Save, Add, Connect, GetTable, SetPK, GetERTable, Delete, Undo, Redo, Exit};
enum Cardinality{CardinalityOptionOne, CardinalityOptionTwo};

class TextUI
{
public:
	TextUI(PresentationModel*);
	virtual ~TextUI();
	void addNewNode();
	void addConnection();
	void setPrimaryKey();
	void displayMenu();
	void displayComponentTable();
	void displayConnectionTable();
	void displayERDiagramTable();
	void processCommand();
	void exitERDiagram();
	void loadERDiagram();
	void saveERDiagram();
	void deleteComponent();
	void undoCmd();
	void redoCmd();
	string chooseCardinality();
	string searchComponent(string);
	string searchEntity(string);
	vector<int> searchAttribute(string);
private:
	// ERModel* _erModel;
	PresentationModel* _presentationModel;
	bool _isDisplayComponentsTable;
	bool _isDisplayConnectionsTable;
};

#endif