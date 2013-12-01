#include "ConnectionState.h"
#include "ERDiagramScene.h"

ConnectionState::ConnectionState( ERDiagramScene* scene, ItemComponent* previewItem ): GuiState(scene)
{
	_sourceItem = NULL;
	_destionationItem = NULL;
// 	_previewItem = previewItem;
// 	_destionationPoint = new ItemEntity(0,0,"");
}

ConnectionState::~ConnectionState()
{
	//delete _destionationPoint;
}

void ConnectionState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// 將點到位置對應到Scene的Item放進來
		_sourceItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));
//		_destionationPoint->setPos(event->scenePos());
	}
// 	static_cast<ItemConnection*>(_previewItem)->setSourceItem(_sourceItem);
// 	static_cast<ItemConnection*>(_previewItem)->setDestionationItem(_destionationPoint);
// 
// 	_scene->addItem(_previewItem);
}

void ConnectionState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
// 	_destionationPoint->setPos(event->scenePos());
}

void ConnectionState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// 將點到位置對應到Scene的Item放進來
		_destionationItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));

// 		if (_destionationItem != NULL)
// 			static_cast<ItemConnection*>(_previewItem)->setDestionationItem(_destionationItem);
// 		else
// 			_destionationPoint->setPos(event->scenePos());
	}

	if (_sourceItem != NULL && _destionationItem != NULL)
		_scene->addConnectionFromGUI(_sourceItem, _destionationItem, "");

	_scene->changToPointerMode();
}
