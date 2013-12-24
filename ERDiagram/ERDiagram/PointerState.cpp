#include "PointerState.h"
#include "ERDiagramScene.h"

PointerState::PointerState( ERDiagramScene* scene ) : GuiState(scene)
{
}

PointerState::~PointerState()
{
}

void PointerState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	_scene->sceneMousePress(event);
}

void PointerState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	_scene->sceneMouseMove(event);
}

void PointerState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	_scene->sceneMouseRelease(event);
}
