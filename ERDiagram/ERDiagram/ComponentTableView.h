#ifndef COMPONENTTABLEVIEW_H_
#define COMPONENTTABLEVIEW_H_

#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndex>

class ComponentTableView : public QTableView
{
public:
	ComponentTableView(QStandardItemModel);
	~ComponentTableView();
	void updateModel(QStandardItemModel);
	private slots:
		void doubleClickedEvent(const QModelIndex&);
private:
	QStandardItemModel _tableViewModel;
};

#endif
