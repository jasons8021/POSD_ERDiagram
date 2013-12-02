#include "ComponentTableView.h"
#include "..\src\corelib\io\qdebug.h"

ComponentTableView::ComponentTableView( QStandardItemModel tableViewModel )
{
	_tableViewModel = tableViewModel;
	setModel(_tableViewModel);

	horizontalHeader()->setStretchLastSection(true);
	resizeColumnsToContents();
	setMinimumWidth(TABLEVIEW_MINWIDTH);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	connect(_tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClickedEvent(const QModelIndex&)));
}

ComponentTableView::~ComponentTableView()
{
}

void ComponentTableView::doubleClickedEvent( const QModelIndex& modelIndex )
{
	qDebug()<<modelIndex;
}

void ComponentTableView::updateModel( QStandardItemModel newTableViewModel )
{
	_tableViewModel = newTableViewModel;

	setModel(_tableViewModel);
	update();
}
