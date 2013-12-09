#ifndef COMPONENT_H_
#define COMPONENT_H_

#define TEXT_NODENUMBEGIN "The node '"
#define TEXT_CONNECTION_NOTCONNECTION "' cannot be connected by the node '"
#define TEXT_CONNECTION_ITSELFCONNECTION "' cannot be connected to itself."
#define TEXT_CONNECTION_ALREADYCONNECTION "' has already been connected to component '"
#define TEXT_ENDTEXT "'."
#define TEXT_CONNECTION_CANCONNECT "CANCONNECT"

#define PARAMETER_NULL ""

#define PARAMETER_TEXTUI_COORDINATES 0

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Component
{
	friend class ComponentTest;
public:
	Component();
	Component(int, string, string, int, int);
	virtual ~Component();
	int getID();
	bool searchConnections(int);
	void pushConnection(Component*);
	string getIDString();
	string getType();
	string getText();
	void setText(string);
	vector<Component*> getConnections();
	virtual void deleteConnectedComponent(int);
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
private:
	int _id;
	string _type;
	string _text;
	int _sx;
	int _sy;
	vector<Component*> _connections;
};

#endif