#include "AddAttributeState.h"
#include "ERDiagramScene.h"

AddAttributeState::AddAttributeState( ERDiagramScene* scene, ItemComponent* previewItem ) : AddNodeState(scene, previewItem)
{
}

AddAttributeState::~AddAttributeState()
{
	delete _previewItem;
}

void AddAttributeState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mousePressEvent(event);
	_scene->addNodeFromGUI(getPreviewItemPosition(event->scenePos())-QPointF(_textWidth/2,0), "A", _text);
	_scene->changToPointerMode();
}

void AddAttributeState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseMoveEvent(event);
}

void AddAttributeState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseReleaseEvent(event);
}