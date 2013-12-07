#include "SetPrimaryKeyState.h"
#include "ERDiagramScene.h"
#include "GUI.h"

SetPrimaryKeyState::SetPrimaryKeyState( GUI* gui, ERDiagramScene* scene ) : GuiState(scene)
{
	_gui = gui;
}

SetPrimaryKeyState::~SetPrimaryKeyState()
{

}

void SetPrimaryKeyState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// 判斷點到的是否為AttributeItem
		if (static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()))->getType() == PARAMETER_ATTRIBUTE)
		{
			_clickedAttributeItem = static_cast<ItemAttribute*>(_scene->itemAt(event->scenePos()));
			_gui->changePrimaryKey(_clickedAttributeItem->getItemID(), !_clickedAttributeItem->getPrimaryKey());
		}
	}

	_scene->changToPointerMode();
}

void SetPrimaryKeyState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

}

void SetPrimaryKeyState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

}
