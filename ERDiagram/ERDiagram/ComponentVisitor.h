#ifndef COMPONENTVISITOR_H_
#define COMPONENTVISITOR_H_

#include "NodeAttribute.h"
#include "NodeEntity.h"
#include "NodeRelationship.h"
#include "Connector.h"

class ComponentVisitor
{
public:
	ComponentVisitor();
	virtual ~ComponentVisitor();
	virtual void visit(NodeAttribute*) = 0;
	virtual void visit(NodeEntity*) = 0;
	virtual void visit(NodeRelationship*) = 0;
	virtual void visit(Connector*) = 0;
};

#endif