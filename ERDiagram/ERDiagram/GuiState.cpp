#include "GuiState.h"
#include "ERDiagramScene.h"

GuiState::GuiState()
{
	
}

GuiState::GuiState( ERDiagramScene* scene )
{
	this->_scene = scene;
}

GuiState::~GuiState()
{
	
}

void GuiState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
}

void GuiState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
}

void GuiState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
}
