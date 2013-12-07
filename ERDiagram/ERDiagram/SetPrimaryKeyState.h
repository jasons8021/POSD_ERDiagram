#ifndef SETPRIMARYKEYSTATE_H_
#define SETPRIMARYKEYSTATE_H_

#define PARAMETER_ATTRIBUTE "A"

#include "GuiState.h"
#include "ItemComponent.h"
#include "ItemAttribute.h"

class GUI;

class SetPrimaryKeyState : public GuiState
{
public:
	SetPrimaryKeyState(GUI*, ERDiagramScene*);
	virtual ~SetPrimaryKeyState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
private:
	ItemAttribute* _clickedAttributeItem;
	GUI* _gui;
};

#endif
