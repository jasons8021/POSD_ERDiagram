#include "ConnectionState.h"
#include "ERDiagramScene.h"

ConnectionState::ConnectionState( ERDiagramScene* scene, ItemComponent* previewItem ) : GuiState(scene)
{
	_sourceItem = NULL;
	_destionationItem = NULL;
}

ConnectionState::~ConnectionState()
{
}

void ConnectionState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// �N�I���m������Scene��Item��i��
		_sourceItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));
	}
}

void ConnectionState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
}

void ConnectionState::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		// �N�I���m������Scene��Item��i��
		_destionationItem = static_cast<ItemComponent*>(_scene->itemAt(event->scenePos()));
	}

	if (_sourceItem != NULL && _destionationItem != NULL)
	{
		// ���Item�OEntity�PRelationship�����Y
		if (_scene->checkSetCardinality(_sourceItem->getItemID(), _destionationItem->getItemID()))
		{
			// ��JCardinality����
			bool dialogFlag;
			QString text = QInputDialog::getText(0, "Enter Cardinality","Please enter 1 or N:", QLineEdit::Normal,"", &dialogFlag, Qt::Dialog);
			if (dialogFlag && !text.isEmpty() && (text == "1" || text == "N"))
				_scene->addConnectionFromGUI(_sourceItem, _destionationItem, text);
			else
			{
				// ��J���O1��N�����w�]��1
				_scene->addConnectionFromGUI(_sourceItem, _destionationItem, "1");
			}
		}
		else
			_scene->addConnectionFromGUI(_sourceItem, _destionationItem, "");
	}
	_scene->changToPointerMode();
}
