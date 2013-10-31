#ifndef ERDIAGRAMSCENE_H_
#define ERDIAGRAMSCENE_H_

#include <QGraphicsScene>
#include "EntityItem.h"
#include "ComponentItem.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class ERDiagramScene : public QGraphicsScene
{
	Q_OBJECT
public:
	ERDiagramScene(QObject* parent = 0);
	virtual ~ERDiagramScene();
};

#endif