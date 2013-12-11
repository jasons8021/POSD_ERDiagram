#include "ComponentTableView.h"

ComponentTableView::ComponentTableView( GUI* gui, QStandardItemModel* tableViewModel )
{
	_gui = gui;
	_clickedIndex = QModelIndex();

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
	_clickedIndex = index;
}

void ComponentTableView::closeEditor( QWidget *editor, QAbstractItemDelegate::EndEditHint hint )
{
	QVariant value = this->model()->data(_clickedIndex,0);
	if (value.isValid()&&_clickedIndex.column()!=0)
		_gui->changeItemText(_clickedIndex.row(), value.toString());

	QTableView::closeEditor(editor,hint);
}

void ComponentTableView::updateModel( QStandardItemModel* newTableViewModel )
{
	_tableViewModel = newTableViewModel;

	setModel(_tableViewModel);
	update();
}
