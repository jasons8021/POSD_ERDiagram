#ifndef SAVECOMPONENTVISITOR_H_
#define SAVECOMPONENTVISITOR_H_

#define TEXT_NULL ""
#define SPACE " "
#define COMMA ","
#define COMMASPACE ", "
#define ENDLINE "\n"

#include "ComponentVisitor.h"

using namespace std;

class SaveComponentVisitor : public ComponentVisitor
{
public:
	SaveComponentVisitor();
	virtual ~SaveComponentVisitor();
	void visit(NodeAttribute*);
	void visit(NodeEntity*);
	void visit(NodeRelationship*);
	void visit(Connector*);
	string getERDiagramComponent();
	string getPositionInfo();
private:
	string _componentInfoString;
	string _connectionInfoString;
	string _primaryKeyInfoString;
	string _positionInfoString;
};

#endif
