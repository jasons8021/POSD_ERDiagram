#include "AddEntityState.h"
#include "ERDiagramScene.h"

AddEntityState::AddEntityState( ERDiagramScene* scene, ItemComponent* previewItem ) : AddNodeState(scene, previewItem)
{

}

AddEntityState::~AddEntityState()
{
	delete _previewItem;
}

void AddEntityState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mousePressEvent(event);

	// 如果有輸入東西才新增
	if (!_text.isEmpty())
		_scene->addNodeFromGUI(getPreviewItemPosition(event->scenePos())-QPointF(_textWidth/2,0), PARAMETER_ENTITY, _text);
	
	_scene->changToPointerMode();
}

void AddEntityState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseMoveEvent(event);
}

void AddEntityState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	AddNodeState::mouseReleaseEvent(event);
}
