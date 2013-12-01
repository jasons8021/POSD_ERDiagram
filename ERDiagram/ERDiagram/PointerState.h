#ifndef POINTERSTATE_H_
#define POINTERSTATE_H_

#include "GuiState.h"
#include "ItemComponent.h"

class ERDiagramScene;

class PointerState : public GuiState
{
public:
	PointerState(ERDiagramScene*);
	virtual ~PointerState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
protected:
	ItemComponent* _clickedItem;
};

#endif