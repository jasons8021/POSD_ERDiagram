#ifndef CONNECTIONSTATE_H_
#define CONNECTIONSTATE_H_

#include "GuiState.h"
#include "ItemComponent.h"

class ConnectionState :	public GuiState
{
public:
	ConnectionState(ERDiagramScene*, ItemComponent*);
	virtual ~ConnectionState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
private:
	ItemComponent* _sourceItem;
	ItemComponent* _destionationItem;
	ItemComponent* _previewItem;
//	ItemComponent* _destionationPoint;
};

#endif


