#ifndef SAVEXMLCOMPONENTVISITOR_H_
#define SAVEXMLCOMPONENTVISITOR_H_

#include "ComponentVisitor.h"

class SaveXmlComponentVisitor :	public ComponentVisitor
{
public:
	SaveXmlComponentVisitor();
	virtual ~SaveXmlComponentVisitor();
	void visit(NodeAttribute*);
	void visit(NodeEntity*);
	void visit(NodeRelationship*);
	void visit(Connector*);
private:
	string _xmlInfoString;
};

#endif