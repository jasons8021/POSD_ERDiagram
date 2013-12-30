#ifndef SAVECOMPONENTVISITOR_H_
#define SAVECOMPONENTVISITOR_H_

#include "ComponentVisitor.h"

class SaveComponentVisitor : public ComponentVisitor
{
public:
	SaveComponentVisitor();
	virtual ~SaveComponentVisitor();
	void visit(NodeAttribute*);
	void visit(NodeEntity*);
	void visit(NodeRelationship*);
	void visit(Connector*);
private:
	string _componentInfoString;
};

#endif
