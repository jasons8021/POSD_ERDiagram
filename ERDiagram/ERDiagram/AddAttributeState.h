#ifndef ADDATTRIBUTESTATE_H_
#define ADDATTRIBUTESTATE_H_

#define PARAMETER_ATTRIBUTE "A"

#include "AddNodeState.h"

class AddAttributeState : public AddNodeState
{
public:
	AddAttributeState(ERDiagramScene*, ItemComponent*);
	virtual ~AddAttributeState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

#endif