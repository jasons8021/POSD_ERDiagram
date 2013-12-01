#ifndef GUISTATE_H_
#define GUISTATE_H_

#include <QGraphicsSceneMouseEvent>

class ERDiagramScene;

class GuiState
{
public:
	GuiState();
	GuiState(ERDiagramScene*);
	virtual ~GuiState();
	virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
protected:
	ERDiagramScene* _scene;
};

#endif