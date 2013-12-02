#include "AddRelationshipState.h"
#include "ERDiagramScene.h"

AddRelationshipState::AddRelationshipState( ERDiagramScene* scene, ItemComponent* previewItem ) : AddNodeState(scene, previewItem)
{

}

AddRelationshipState::~AddRelationshipState()
{
	delete _previewItem;
}

void AddRelationshipState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mousePressEvent(event);
	_scene->addNodeFromGUI(getPreviewItemPosition(event->scenePos())-QPointF(_textWidth/2,0), PARAMETER_RELATIONSHIP, _text);
	_scene->changToPointerMode();
}

void AddRelationshipState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseMoveEvent(event);
}

void AddRelationshipState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseReleaseEvent(event);
}
