#ifndef SAVEXMLCOMPONENTVISITOR_H_
#define SAVEXMLCOMPONENTVISITOR_H_

#define XMLTITLE "<?xml version=\"1.0\"?>"
#define TEXT_NULL ""
#define ENDLINE "\n"
#define TAG "\t"
#define DOUBLETAG "\t\t"
#define COMMA ","

#define START_ERDIAGRAM "<ERDiagram>"
#define END_ERDIAGRAM "</ERDiagram>"

#define START_ATTRIBUTE "<Attribute>"
#define END_ATTRIBUTE "</Attribute>"

#define START_ENTITY "<Entity>"
#define END_ENTITY "</Entity>"

#define START_RELATIONSHIP "<Relation>"
#define END_RELATIONSHIP "</Relation>"

#define START_CONNECTOR "<Connector>"
#define END_CONNECTOR "</Connector>"

#define START_ID "<Id>"
#define END_ID "</Id>"

#define START_TEXT "<Text>"
#define END_TEXT "</Text>"

#define START_PRIMARYKEY "<PrimaryKey>"
#define END_PRIMARYKEY "</PrimaryKey>"

#define START_POSITIONX "<positionX>"
#define END_POSITIONX "</positionX>"

#define START_POSITIONY "<positionY>"
#define END_POSITIONY "</positionY>"

#define START_SOURCE "<Sourcr>"
#define END_SOURCE "</Sourcr>"

#define START_DESTINATION "<Target>"
#define END_DESTINATION "</Target>"

#include "ComponentVisitor.h"

using namespace std;

class SaveXmlComponentVisitor :	public ComponentVisitor
{
public:
	SaveXmlComponentVisitor();
	virtual ~SaveXmlComponentVisitor();
	void visit(NodeAttribute*);
	void visit(NodeEntity*);
	void visit(NodeRelationship*);
	void visit(Connector*);
	string getXmlInfo();
private:
	string _xmlInfoString;
};

#endif