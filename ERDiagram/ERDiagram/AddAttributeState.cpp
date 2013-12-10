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

	// 如果有輸入東西才新增
	if (!_text.isEmpty())
		_scene->addNodeFromGUI(PARAMETER_ATTRIBUTE, _text, getPreviewItemPosition(event->scenePos())-QPointF(_textWidth/2,0));

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