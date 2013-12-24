#include "PointerState.h"
#include "ERDiagramScene.h"

PointerState::PointerState( ERDiagramScene* scene ) : GuiState(scene)
{
	_clickedItem = NULL;
}

PointerState::~PointerState()
{
}

void PointerState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// 將點到位置對應到Scene的Item放進來
		_clickedItem = _scene->getItemInPosition(event->scenePos());

		// 如果點到沒東西，不作用
		if (_clickedItem != NULL)
		{
			// 去call那個Item的mouse事件
			_clickedItem->mousePressEvent(event);
		}
	}
}

void PointerState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	if (_clickedItem != NULL)
		_clickedItem->mouseMoveEvent(event);
}

void PointerState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	if (_clickedItem != NULL)
		_clickedItem->mouseReleaseEvent(event);
}
