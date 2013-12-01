#ifndef ADDENTITYSTATE_H_
#define ADDENTITYSTATE_H_

#include "AddNodeState.h"

class AddEntityState :public AddNodeState
{
public:
	AddEntityState(ERDiagramScene*, ItemComponent*);
	virtual ~AddEntityState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

#endif

