#include "AddNodeState.h"
#include "ERDiagramScene.h"

AddNodeState::AddNodeState( ERDiagramScene* scene, ItemComponent* previewItem ) : GuiState(scene)
{
	_previewItem = previewItem;
	_textWidth = 0;
}

AddNodeState::~AddNodeState()
{

}

void AddNodeState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	QFont textFont;
	QFontMetrics textFM(textFont);

	bool dialogFlag;
	QString text = QInputDialog::getText(0, "Enter Text","Please enter the text:", QLineEdit::Normal,"", &dialogFlag, Qt::Dialog);
	if (dialogFlag && !text.isEmpty())
	{
		_text = text;
		_textWidth = textFM.width(_text);
	}

	setPreviewItemPosition(event->scenePos());
}

void AddNodeState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	setPreviewItemPosition(event->scenePos());
	_scene->addItem(_previewItem);
}

void AddNodeState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	setPreviewItemPosition(event->scenePos());
}

void AddNodeState::setPreviewItemPosition( QPointF mousePoint )
{
	_previewItem->setPos(getPreviewItemPosition(mousePoint));
}

QPointF AddNodeState::getPreviewItemPosition( QPointF mousePoint )
{
	return (mousePoint - _previewItem->getItemCenter());
}
