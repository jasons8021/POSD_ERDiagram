#ifndef ADDRELATIONSHIPSTATE_H_
#define ADDRELATIONSHIPSTATE_H_

#define PARAMETER_RELATIONSHIP "R"

#include "AddNodeState.h"

class AddRelationshipState : public AddNodeState
{
public:
	AddRelationshipState(ERDiagramScene*, ItemComponent*);
	virtual ~AddRelationshipState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

#endif
