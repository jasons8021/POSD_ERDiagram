#include "ComponentTableView.h"
#include "..\src\corelib\io\qdebug.h"

ComponentTableView::ComponentTableView( QStandardItemModel* tableViewModel )
{
	// header的對齊設定
	setColumnWidth (COLUMN_TYPE,COLUMN_WIDTH);
	setColumnWidth (COLUMN_TEXT,COLUMN_WIDTH);
	resizeColumnsToContents();
	setMinimumWidth(TABLEVIEW_MINWIDTH);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClickedEvent(const QModelIndex&)));

	// Load TableView的Model
	_tableViewModel = tableViewModel;
	setModel(_tableViewModel);
}

ComponentTableView::~ComponentTableView()
{
}

void ComponentTableView::doubleClickedEvent( const QModelIndex& index )
{
	qDebug()<<index;
}

void ComponentTableView::updateModel( QStandardItemModel* newTableViewModel )
{
	_tableViewModel = newTableViewModel;

	setModel(_tableViewModel);
	update();
}
