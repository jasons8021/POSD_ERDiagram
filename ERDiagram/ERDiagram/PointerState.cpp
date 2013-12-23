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
		// �N�I���m������Scene��Item��i��
		_clickedItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));
		// �p�G�I��S�F��A���@��
		//qDebug()<<"-"<<event->scenePos().rx()<< "  "<<event->scenePos().ry();
		if (_clickedItem != NULL)
		{
			// �hcall����Item��mouse�ƥ�
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
