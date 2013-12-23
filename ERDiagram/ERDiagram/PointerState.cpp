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
		_clickedItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));
		// 如果點到沒東西，不作用
		//qDebug()<<"-"<<event->scenePos().rx()<< "  "<<event->scenePos().ry();
		if (_clickedItem != NULL)
		{
			// 去call那個Item的mouse事件
			_clickedItem->mousePressEvent(event);
			//qDebug()<<event->scenePos().rx()<< "  "<<event->scenePos().ry();

			//qDebug()<<_clickedItem->getItemID();
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
