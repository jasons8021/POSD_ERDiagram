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

	// 如果有輸入東西才新增
	if (!_text.isEmpty())
		_scene->addNodeFromGUI(PARAMETER_RELATIONSHIP, _text, getPreviewItemPosition(event->scenePos())-QPointF(_textWidth/2,0));
	
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
