#ifndef ADDNODESTATE_H_
#define ADDNODESTATE_H_

#include <QInputDialog>
#include "GuiState.h"
#include "ItemComponent.h"

class AddNodeState : public GuiState
{
public:
	AddNodeState(ERDiagramScene*, ItemComponent*);
	virtual ~AddNodeState();
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
	void setPreviewItemPosition(QPointF);
	QPointF getPreviewItemPosition(QPointF);
protected:
	int _textWidth;
	ItemComponent* _previewItem;
	QString _text;
};

#endif

