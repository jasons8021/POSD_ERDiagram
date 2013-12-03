#ifndef COMPONENTTABLEVIEW_H_
#define COMPONENTTABLEVIEW_H_

#define TABLEVIEW_MINWIDTH 210
#define TABLEVIEW_MAXWIDTH 400
#define COLUMN_TYPE 0
#define COLUMN_TEXT 1
#define COLUMN_WIDTH 100

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QModelIndex>

class ComponentTableView : public QTableView
{
	Q_OBJECT
public:
	ComponentTableView(QStandardItemModel*);
	virtual ~ComponentTableView();
	void updateModel(QStandardItemModel*);
	private slots:
		void doubleClickedEvent(const QModelIndex&);
private:
	QStandardItemModel* _tableViewModel;
};

#endif
